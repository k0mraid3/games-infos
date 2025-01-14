
using Newtonsoft.Json.Linq;
using System;
using System.Collections.Generic;
using System.Globalization;
using System.IO;
using System.Linq;
using System.Net;
using System.Text;
using System.Threading.Tasks;
using System.Web;

namespace EGS
{
    class WebApi : IDisposable
    {
        public const string EPIC_GAMES_HOST = "www.epicgames.com";
        public const string EGL_UAGENT = "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) EpicGamesLauncher/12.2.4-16388143+++Portal+Release-Live UnrealEngine/4.23.0-14907503+++Portal+Release-Live Chrome/84.0.4147.38 Safari/537.36";
        public const string EGS_OAUTH_UAGENT = "EpicGamesLauncher/12.2.4-16388143+++Portal+Release-Live Windows/10.0.19041.1.256.64bit";
        public const string EGS_USER = "34a02cf8f4414e29b15921876da36f9a";
        public const string EGS_PASS = "daafbccc737745039dffe53d94fc76cf";
        public const string EGS_OAUTH_HOST = "account-public-service-prod03.ol.epicgames.com";
        public const string EGS_LAUNCHER_HOST = "launcher-public-service-prod06.ol.epicgames.com";
        public const string EGS_ENTITLEMENT_HOST = "entitlement-public-service-prod08.ol.epicgames.com";
        public const string EGS_CATALOG_HOST = "catalog-public-service-prod06.ol.epicgames.com";
        public const string EGS_DEV_HOST = "api.epicgames.dev";

        CookieWebClient _WebCli = new CookieWebClient();
        string _SessionID = string.Empty;
        JObject _OAuthInfos = new JObject();
        bool _LoggedIn = false;

        public void Dispose()
        {
            _WebCli.Dispose();
        }

        void _ResetOAuth()
        {
            _OAuthInfos = new JObject();
            _LoggedIn = false;
        }

        void _UpdateOAuth(JObject oauth_infos)
        {
            _OAuthInfos["access_token"] = oauth_infos["token"];
            _OAuthInfos["expires_in"] = oauth_infos["expires_in"];
            _OAuthInfos["expires_at"] = oauth_infos["expires_at"];
            _OAuthInfos["token_type"] = oauth_infos["token_type"];
            _OAuthInfos["account_id"] = oauth_infos["account_id"];
            _OAuthInfos["client_id"] = oauth_infos["client_id"];
            _OAuthInfos["internal_client"] = oauth_infos["internal_client"];
            _OAuthInfos["client_service"] = oauth_infos["client_service"];
            _OAuthInfos["display_name"] = oauth_infos["display_name"];
            _OAuthInfos["app"] = oauth_infos["app"];
            _OAuthInfos["in_app_id"] = oauth_infos["in_app_id"];
            _OAuthInfos["device_id"] = oauth_infos["device_id"];
            _SessionID = (string)oauth_infos["session_id"];
        }

        Error _GetWebErrorFromException(Exception e)
        {
            Error err = new Error();
            if (e is WebException && ((WebException)e).Response != null)
            {
                WebException we = (WebException)e;
                try
                {
                    int error_code = 0;
                    string error_name = string.Empty;
                    string message = string.Empty;

                    if(we.Response.Headers.AllKeys.Contains("X-Epic-Error-Code"))
                    {
                        error_code = int.Parse(we.Response.Headers["X-Epic-Error-Code"]);
                    }
                    if (we.Response.Headers.AllKeys.Contains("X-Epic-Error-Name"))
                    {
                        error_name = we.Response.Headers["X-Epic-Error-Name"];
                    }

                    JObject json = null;
                    try
                    {
                        using (StreamReader reader = new StreamReader(we.Response.GetResponseStream()))
                        {
                            json = JObject.Parse(reader.ReadToEnd());
                        }
                        if (json.ContainsKey("message"))
                            message = (string)json["message"];
                        else if (json.ContainsKey("errorMessage"))
                            message = (string)json["errorMessage"];

                        if (error_code == 0 && json.ContainsKey("numericErrorCode"))
                        {
                            error_code = (int)json["numericErrorCode"];
                        }
                        if (string.IsNullOrWhiteSpace(error_name) && json.ContainsKey("errorCode"))
                        {
                            error_name = (string)json["errorCode"];
                        }
                    }
                    catch(Exception)
                    { }

                    err.Message = string.IsNullOrWhiteSpace(message) ? error_name : message;

                    if (error_code == 0 && !string.IsNullOrWhiteSpace(error_name))
                    {
                        switch (error_name)
                        {
                            case "errors.com.epicgames.common.authentication.authentication_failed": err.ErrorCode = Error.CommonAuthenticationAuthenticationFailed; break;
                            case "errors.com.epicgames.accountportal.session_id_invalid": err.ErrorCode = Error.AccountPortalSessionIdInvalid; break;
                            case "errors.com.epicgames.accountportal.validation": err.ErrorCode = Error.AccountPortalValidation; break;
                            case "errors.com.epicgames.accountportal.csrf_token_invalid": err.ErrorCode = Error.AccountPortalCsrfTokenInvalid; break;
                            case "errors.com.epicgames.common.method_not_allowed": err.ErrorCode = Error.CommonMethodNotAllowed; break;
                            case "errors.com.epicgames.account.oauth.exchange_code_not_found": err.ErrorCode = Error.AccountOauthExchangeCodeNotFound; break;
                            case "errors.com.epicgames.common.oauth.invalid_token": err.ErrorCode = Error.CommonOauthInvalidToken; break;
                            case "errors.com.epicgames.account.auth_token.invalid_refresh_token": err.ErrorCode = Error.AccountAuthTokenInvalidRefreshToken; break;
                            default: err.ErrorCode = Error.Unknown; break;
                        }
                    }
                    else
                    {
                        err.ErrorCode = Error.Unknown;
                    }
                }
                catch (Exception)
                {
                    err.Message = e.Message;
                    err.ErrorCode = Error.WebError;
                }
            }
            else
            {
                err.Message = e.Message;
                err.ErrorCode = Error.WebError;
            }

            return err;
        }

        async Task<Error<string>> _GetXSRFToken()
        {
            Error<string> err = new Error<string> { ErrorCode = 0 };

            try
            {
                Uri uri = new Uri(string.Format("https://{0}/id/api/csrf", EPIC_GAMES_HOST));

                _WebCli.Headers["User-Agent"] = EGS_OAUTH_UAGENT;
                string response = await _WebCli.DownloadStringTaskAsync(uri);

                foreach (Cookie c in _WebCli.CookieContainer.GetCookies(uri))
                {
                    if (c.Name == "XSRF-TOKEN")
                    {
                        err.Result = c.Value;
                        return err;
                    }
                }

                err.Message = "XSRF-TOKEN cookie not found.";
                err.ErrorCode = Error.NotFound;
            }
            catch (Exception e)
            {
                Error werr = _GetWebErrorFromException(e);
                err.ErrorCode = werr.ErrorCode;
                err.Message = werr.Message;
            }

            return err;
        }

        async Task<Error<string>> _GetExchangeCode(string xsrf_token)
        {
            Error<string> err = new Error<string> { ErrorCode = 0 };

            try
            {
                Uri uri = new Uri(string.Format("https://{0}/id/api/exchange/generate", EPIC_GAMES_HOST));

                _WebCli.Headers["User-Agent"] = EGS_OAUTH_UAGENT;
                _WebCli.Headers["X-XSRF-TOKEN"] = xsrf_token;

                System.Collections.Specialized.NameValueCollection post_datas = new System.Collections.Specialized.NameValueCollection();

                JObject response = JObject.Parse(Encoding.UTF8.GetString(await _WebCli.UploadValuesTaskAsync(uri, "POST", post_datas)));

                try
                {
                    err.Result = (string)response["code"];
                }
                catch (Exception)
                {
                    err.Message = "Exchange code not found.";
                    err.ErrorCode = Error.NotFound;
                }
            }
            catch (Exception e)
            {
                Error werr = _GetWebErrorFromException(e);
                err.ErrorCode = werr.ErrorCode;
                err.Message = werr.Message;
            }

            return err;
        }

        async Task<Error> _ResumeSession(string access_token)
        {
            Error err = new Error { ErrorCode = 0 };

            _WebCli.ClearCookies();
            _WebCli.Headers.Clear();
            _WebCli.Headers["User-Agent"] = EGS_OAUTH_UAGENT;
            _WebCli.Headers["Accept"] = "*/*";
            _WebCli.Headers["Authorization"] = access_token;

            try
            {
                Uri uri = new Uri(string.Format("https://{0}/account/api/oauth/verify", EGS_OAUTH_HOST));
                JObject oauth = JObject.Parse(await _WebCli.DownloadStringTaskAsync(uri));
                _UpdateOAuth(oauth);
            }
            catch (Exception e)
            {
                Error werr = _GetWebErrorFromException(e);
                err.ErrorCode = werr.ErrorCode;
                err.Message = werr.Message;
            }

            return err;
        }

        async Task<Error<JObject>> _StartSession(AuthToken token)
        {
            Error<JObject> err = new Error<JObject> { ErrorCode = 0 };
            System.Collections.Specialized.NameValueCollection post_data;
            switch (token.Type)
            {
                case AuthToken.TokenType.ExchangeCode:
                    post_data = new System.Collections.Specialized.NameValueCollection()
                    {
                        { "grant_type"   , "exchange_code" },
                        { "exchange_code", token.Token },
                        { "token_type"   , "eg1"},
                    };
                    break;

                case AuthToken.TokenType.RefreshToken:
                    post_data = new System.Collections.Specialized.NameValueCollection()
                    {
                        { "grant_type"   , "refresh_token" },
                        { "refresh_token", token.Token },
                        { "token_type"   , "eg1"},
                    };
                    break;

                default:
                    err.Message = "Invalid token type.";
                    err.ErrorCode = Error.InvalidParam;
                    return err;
            }

            _WebCli.ClearCookies();
            _WebCli.Headers.Clear();
            _WebCli.Headers["User-Agent"] = EGS_OAUTH_UAGENT;
            _WebCli.Headers["Accept-Encoding"] = "gzip, deflate";
            _WebCli.Headers["Accept"] = "*/*";
            _WebCli.Headers["Authorization"] = string.Format("Basic {0}", Convert.ToBase64String(Encoding.UTF8.GetBytes(string.Format("{0}:{1}", EGS_USER, EGS_PASS))));

            try
            {
                Uri uri = new Uri(string.Format("https://{0}/account/api/oauth/token", EGS_OAUTH_HOST));
                err.Result = JObject.Parse(Encoding.UTF8.GetString(await _WebCli.UploadValuesTaskAsync(uri, "POST", post_data)));
            }
            catch (Exception e)
            {
                Error werr = _GetWebErrorFromException(e);
                err.ErrorCode = werr.ErrorCode;
                err.Message = werr.Message;
            }

            return err;
        }

        public async Task<Error<JObject>> LoginSID(string sid)
        {
            Error<JObject> err = new Error<JObject>();

            _ResetOAuth();

            _WebCli.ClearCookies();
            _WebCli.Headers.Clear();
            _WebCli.Headers["User-Agent"] = EGL_UAGENT;
            _WebCli.Headers["X-Epic-Event-Action"] = "login";
            _WebCli.Headers["X-Epic-Event-Category"] = "login";
            _WebCli.Headers["X-Requested-With"] = "XMLHttpRequest";

            try
            {
                Uri uri = new Uri(string.Format("https://{0}/id/api/set-sid?sid={1}", EPIC_GAMES_HOST, sid));
                string response = await _WebCli.DownloadStringTaskAsync(uri);

                string xsrf_token;
                {
                    Error<string> x = await _GetXSRFToken();
                    if (x.ErrorCode != Error.OK)
                    {
                        err.ErrorCode = x.ErrorCode;
                        err.Message = x.Message;
                        return err;
                    }
                    xsrf_token = x.Result;
                }

                string exchange_code;
                {
                    Error<string> x = await _GetExchangeCode(xsrf_token);
                    if (x.ErrorCode != Error.OK)
                    {
                        err.ErrorCode = x.ErrorCode;
                        err.Message = x.Message;
                        return err;
                    }
                    exchange_code = x.Result;
                }

                AuthToken auth_token = new AuthToken { Token = exchange_code, Type = AuthToken.TokenType.ExchangeCode };
                {
                    Error<JObject> x = await _StartSession(auth_token);
                    if (x.ErrorCode != Error.OK)
                    {
                        err.ErrorCode = x.ErrorCode;
                        err.Message = x.Message;
                        return err;
                    }
                    _OAuthInfos = x.Result;
                }

                string access_token = string.Format("bearer {0}", (string)_OAuthInfos["access_token"]);
                {
                    Error x = await _ResumeSession(access_token);
                    if (x.ErrorCode != Error.OK)
                    {
                        err.ErrorCode = x.ErrorCode;
                        err.Message = x.Message;
                        return err;
                    }
                    // Don't share our internal oauth infos with the user returned oauth.
                    err.Result = (JObject)_OAuthInfos.DeepClone();
                    err.ErrorCode = Error.OK;
                    _LoggedIn = true;
                }
            }
            catch (Exception e)
            {
                Error werr = _GetWebErrorFromException(e);
                err.ErrorCode = werr.ErrorCode;
                err.Message = werr.Message;
            }

            return err;
        }

        public async Task<Error<JObject>> Login(JObject oauth_infos)
        {
            Error<JObject> err = new Error<JObject> { ErrorCode = Error.InvalidParam, Message = "Invalid cached oauth infos." };

            _ResetOAuth();

            try
            {
                if (!oauth_infos.ContainsKey("access_token") || !oauth_infos.ContainsKey("expires_at"))
                {
                    err.ErrorCode = Error.InvalidParam;
                    err.Message = string.Format("OAuth credentials is missing datas.");
                    return err;
                }

                DateTime dt = new DateTime();
                {
                    JToken date_token = oauth_infos["expires_at"];
                    if (date_token.Type == JTokenType.String)
                    {
                        dt = DateTimeOffset.ParseExact(
                          (string)oauth_infos["expires_at"],
                          new string[] { "yyyy-MM-dd'T'HH:mm:ss.FFFK" },
                          CultureInfo.InvariantCulture,
                          DateTimeStyles.None).UtcDateTime;
                    }
                    else if (date_token.Type == JTokenType.Date)
                    {
                        dt = (DateTime)date_token;
                    }
                    else
                    {
                        err.ErrorCode = Error.InvalidParam;
                        err.Message = string.Format("OAuth credentials 'expires_at' is not an ISO8601 date.");
                        return err;
                    }
                }

                if (dt > DateTime.Now && (dt - DateTime.Now) > TimeSpan.FromMinutes(10))
                {
                    string access_token = string.Format("bearer {0}", (string)oauth_infos["access_token"]);
                    _OAuthInfos = (JObject)oauth_infos.DeepClone();
                    Error x = await _ResumeSession(access_token);
                    if (x.ErrorCode == Error.OK)
                    {
                        // Don't share our internal oauth infos with the user returned oauth.
                        err.Result = (JObject)_OAuthInfos.DeepClone();
                        err.ErrorCode = Error.OK;
                        _LoggedIn = true;
                        return err;
                    }
                }

                AuthToken token = new AuthToken { Token = (string)oauth_infos["refresh_token"], Type = AuthToken.TokenType.RefreshToken };
                {
                    Error<JObject> x = await _StartSession(token);
                    if (x.ErrorCode != Error.OK)
                    {
                        err.Message = x.Message;
                        err.ErrorCode = x.ErrorCode;
                        return err;
                    }
                    _OAuthInfos = x.Result;
                }
                {
                    string access_token = string.Format("bearer {0}", (string)_OAuthInfos["access_token"]);
                    Error x = await _ResumeSession(access_token);
                    if (x.ErrorCode != Error.OK)
                    {
                        err.Message = x.Message;
                        err.ErrorCode = x.ErrorCode;
                        return err;
                    }

                    // Don't share our internal oauth infos with the user returned oauth.
                    err.Result = (JObject)_OAuthInfos.DeepClone();
                    err.ErrorCode = Error.OK;
                    _LoggedIn = true;
                }
            }
            catch (Exception e)
            {
                Error werr = _GetWebErrorFromException(e);
                err.ErrorCode = werr.ErrorCode;
                err.Message = werr.Message;
            }

            return err;
        }

        public async Task<Error> Logout()
        {
            Error err = new Error { ErrorCode = Error.OK };
            if (_LoggedIn)
            {
                try
                {
                    Uri uri = new Uri(string.Format("https://{0}/account/api/oauth/sessions/kill/{1}", EGS_OAUTH_HOST, (string)_OAuthInfos["access_token"]));
                    _WebCli.Headers["Authorization"] = string.Format("bearer {0}", (string)_OAuthInfos["access_token"]);
                    string response = await _WebCli.UploadStringTaskAsync(uri, "DELETE", string.Empty);
                }
                catch (Exception e)
                {
                    Error werr = _GetWebErrorFromException(e);
                    err.ErrorCode = werr.ErrorCode;
                    err.Message = werr.Message;
                }

                _ResetOAuth();
            }

            return err;
        }

        public async Task<Error<string>> GetAppExchangeCode()
        {
            Error<string> err = new Error<string>();
            if (!_LoggedIn)
            {
                err.ErrorCode = Error.NotLoggedIn;
                err.Message = "User is not logged in.";
                return err;
            }

            try
            {
                Uri uri = new Uri(string.Format("https://{0}/account/api/oauth/exchange", EGS_OAUTH_HOST));
                _WebCli.Headers["Authorization"] = string.Format("bearer {0}", (string)_OAuthInfos["access_token"]);
                JObject response = JObject.Parse(await _WebCli.DownloadStringTaskAsync(uri));
                err.Result = (string)response["code"];
                err.ErrorCode = Error.OK;
            }
            catch (Exception e)
            {
                Error werr = _GetWebErrorFromException(e);
                err.ErrorCode = werr.ErrorCode;
                err.Message = werr.Message;
            }

            return err;
        }

        /// <summary>
        /// Get the refresh token that can be used to start a game.
        /// </summary>
        /// <param name="exchange_code">Exchange code generated by GetAppExchangeCode.</param>
        /// <param name="deployement_id">Application DeploymentId.</param>
        /// <param name="user_id">Application ClientId.</param>
        /// <param name="password">Application ClientSecret.</param>
        /// <returns></returns>
        public async Task<Error<string>> GetAppRefreshToken(string exchange_code, string deployement_id, string user_id, string password)
        {
            Error<string> err = new Error<string>();
            if (!_LoggedIn)
            {
                err.ErrorCode = Error.NotLoggedIn;
                err.Message = "User is not logged in.";
                return err;
            }

            try
            {
                System.Collections.Specialized.NameValueCollection post_datas = new System.Collections.Specialized.NameValueCollection()
                {
                    { "grant_type", "exchange_code" },
                    { "exchange_code", exchange_code },
                    { "scope", "basic_profile friends_list presence" },
                    { "deployment_id", deployement_id },
                };

                _WebCli.Headers["Authorization"] = string.Format("Basic {0}", Convert.ToBase64String(Encoding.UTF8.GetBytes(string.Format("{0}:{1}", user_id, password))));

                Uri uri = new Uri(string.Format("https://{0}/epic/oauth/v1/token", EGS_DEV_HOST));
                JObject response = JObject.Parse(UTF8Encoding.UTF8.GetString(await _WebCli.UploadValuesTaskAsync(uri, "POST", post_datas)));
                err.Result = (string)response["refresh_token"];
                err.ErrorCode = Error.OK;
            }
            catch (Exception e)
            {
                Error werr = _GetWebErrorFromException(e);
                err.ErrorCode = werr.ErrorCode;
                err.Message = werr.Message;
            }

            return err;
        }

        public Error<string> GetGameExchangeCodeCommandLine(string exchange_code, string appid)
        {
            Error<string> err = new Error<string>();

            if (!_LoggedIn)
            {
                err.ErrorCode = Error.NotLoggedIn;
                err.Message = "User is not logged in.";
                return err;
            }

            if (!_OAuthInfos.ContainsKey("display_name"))
            {
                err.ErrorCode = Error.NotFound;
                err.Message = "OAuth infos doesn't contain 'display_name'.";
                return err;
            }

            if (!_OAuthInfos.ContainsKey("account_id"))
            {
                err.ErrorCode = Error.NotFound;
                err.Message = "OAuth infos doesn't contain 'account_id'.";
                return err;
            }

            try
            {
                err.Result = string.Format("-AUTH_LOGIN=unused -AUTH_PASSWORD={0} -AUTH_TYPE=exchangecode -epicapp={1} -epicenv=Prod -EpicPortal -epicusername={2} -epicuserid={3} -epiclocal=en", exchange_code, appid, (string)_OAuthInfos["display_name"], (string)_OAuthInfos["account_id"]);
            }
            catch (Exception e)
            {
                err.ErrorCode = Error.InvalidParam;
                err.Message = e.Message;
            }

            return err;
        }

        public Error<string> GetGameTokenCommandLine(string refresh_token, string appid)
        {
            Error<string> err = new Error<string>();

            if (!_LoggedIn)
            {
                err.ErrorCode = Error.NotLoggedIn;
                err.Message = "User is not logged in.";
                return err;
            }

            if (!_OAuthInfos.ContainsKey("display_name"))
            {
                err.ErrorCode = Error.NotFound;
                err.Message = "OAuth infos doesn't contain 'display_name'.";
                return err;
            }

            if (!_OAuthInfos.ContainsKey("account_id"))
            {
                err.ErrorCode = Error.NotFound;
                err.Message = "OAuth infos doesn't contain 'account_id'.";
                return err;
            }

            try
            {
                err.Result = string.Format("-AUTH_LOGIN=unused -AUTH_PASSWORD={0} -AUTH_TYPE=refreshtoken -epicapp={1} -epicenv=Prod -EpicPortal -epicusername={2} -epicuserid={3} -epiclocal=en", refresh_token, appid, (string)_OAuthInfos["display_name"], (string)_OAuthInfos["account_id"]);
            }
            catch (Exception e)
            {
                err.ErrorCode = Error.InvalidParam;
                err.Message = e.Message;
            }

            return err;
        }

        public async Task<Error<List<AppAsset>>> GetGamesAssets(string platform = "Windows", string label = "Live")
        {
            Error<List<AppAsset>> err = new Error<List<AppAsset>>();

            if (!_LoggedIn)
            {
                err.ErrorCode = Error.NotLoggedIn;
                err.Message = "User is not logged in.";
                return err;
            }

            try
            {
                System.Collections.Specialized.NameValueCollection get_datas = new System.Collections.Specialized.NameValueCollection()
                {
                    { "label", label },
                };
                string q = string.Join("&", get_datas.AllKeys.Select(a => a + "=" + HttpUtility.UrlEncode(get_datas[a])));
                Uri uri = new Uri(string.Format("https://{0}/launcher/api/public/assets/{1}?{2}", EGS_LAUNCHER_HOST, platform, q));

                _WebCli.Headers["Authorization"] = string.Format("bearer {0}", (string)_OAuthInfos["access_token"]);
                JArray response = JArray.Parse(await _WebCli.DownloadStringTaskAsync(uri));
                List<AppAsset> app_assets = new List<AppAsset>();

                foreach (JObject asset in response)
                {
                    app_assets.Add(asset.ToObject<AppAsset>());
                }

                err.Result = app_assets;
                err.ErrorCode = Error.OK;
            }
            catch (Exception e)
            {
                Error werr = _GetWebErrorFromException(e);
                err.ErrorCode = werr.ErrorCode;
                err.Message = werr.Message;
            }

            return err;
        }

        public async Task<Error<JObject>> GetGameManifest(string game_namespace, string catalog_id, string app_name, string platform = "Windows", string label = "Live")
        {
            Error<JObject> err = new Error<JObject>();

            if (!_LoggedIn)
            {
                err.ErrorCode = Error.NotLoggedIn;
                err.Message = "User is not logged in.";
                return err;
            }

            try
            {
                Uri uri = new Uri(string.Format("https://{0}/launcher/api/public/assets/v2/platform/{1}/namespace/{2}/catalogItem/{3}/app/{4}/label/{5}", EGS_LAUNCHER_HOST, platform, game_namespace, catalog_id, app_name, label));
                _WebCli.Headers["Authorization"] = string.Format("bearer {0}", (string)_OAuthInfos["access_token"]);

                err.Result = JObject.Parse(await _WebCli.DownloadStringTaskAsync(uri));
                err.ErrorCode = Error.OK;
            }
            catch (Exception e)
            {
                Error werr = _GetWebErrorFromException(e);
                err.ErrorCode = werr.ErrorCode;
                err.Message = werr.Message;
            }

            return err;
        }

        public async Task<Error<List<Entitlement>>> GetUserEntitlements(uint start = 0, uint count = 5000)
        {
            Error<List<Entitlement>> err = new Error<List<Entitlement>>();

            if (!_LoggedIn)
            {
                err.ErrorCode = Error.NotLoggedIn;
                err.Message = "User is not logged in.";
                return err;
            }

            try
            {
                System.Collections.Specialized.NameValueCollection get_datas = new System.Collections.Specialized.NameValueCollection()
                {
                    { "start", start.ToString() },
                    { "count", count.ToString() },
                };
                string q = string.Join("&", get_datas.AllKeys.Select(a => a + "=" + HttpUtility.UrlEncode(get_datas[a])));
                Uri uri = new Uri(string.Format("https://{0}/entitlement/api/account/{1}/entitlements?{2}", EGS_ENTITLEMENT_HOST, (string)_OAuthInfos["account_id"], q));
                _WebCli.Headers["Authorization"] = string.Format("bearer {0}", (string)_OAuthInfos["access_token"]);

                JArray response = JArray.Parse(await _WebCli.DownloadStringTaskAsync(uri));

                List<Entitlement> entitlements = new List<Entitlement>();
                foreach (JObject entitlement in response)
                {
                    entitlements.Add(entitlement.ToObject<EGS.Entitlement>());
                }

                err.Result = entitlements;
                err.ErrorCode = Error.OK;
            }
            catch (Exception e)
            {
                Error werr = _GetWebErrorFromException(e);
                err.ErrorCode = werr.ErrorCode;
                err.Message = werr.Message;
            }

            return err;
        }

        public async Task<Error<AppInfos>> GetGameInfos(string game_namespace, string catalog_item_id, bool include_dlcs = true)
        {
            Error<AppInfos> err = new Error<AppInfos>();

            if (!_LoggedIn)
            {
                err.ErrorCode = Error.NotLoggedIn;
                err.Message = "User is not logged in.";
                return err;
            }

            try
            {
                System.Collections.Specialized.NameValueCollection get_datas = new System.Collections.Specialized.NameValueCollection()
                {
                    { "id", catalog_item_id },
                    { "includeDLCDetails", include_dlcs.ToString() },
                    { "includeMainGameDetails", "true" },
                    { "country", "US" },
                    { "locale", "en" }
                };
                string q = string.Join("&", get_datas.AllKeys.Select(a => a + "=" + HttpUtility.UrlEncode(get_datas[a])));

                Uri uri = new Uri(string.Format("https://{0}/catalog/api/shared/namespace/{1}/bulk/items?{2}", EGS_CATALOG_HOST, game_namespace, q));
                _WebCli.Headers["Authorization"] = string.Format("bearer {0}", (string)_OAuthInfos["access_token"]);

                JObject response = JObject.Parse(await _WebCli.DownloadStringTaskAsync(uri));

                foreach (KeyValuePair<string, JToken> v in response)
                {
                    err.Result = ((JObject)v.Value).ToObject<AppInfos>();
                }

                err.ErrorCode = Error.OK;
            }
            catch (Exception e)
            {
                Error werr = _GetWebErrorFromException(e);
                err.ErrorCode = werr.ErrorCode;
                err.Message = werr.Message;
            }

            return err;
        }
    }
}