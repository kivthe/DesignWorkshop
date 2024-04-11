#ifndef DW_HTTP_H
#define DW_HTTP_H

namespace dw {

enum class HttpRequest : int {
  GET     = 1,
  HEAD    = 2,
  POST    = 3,
  PUT     = 4,
  DELETE  = 5,
  CONNECT = 6,
  OPTIONS = 6,
  TRACE   = 7,
  PATCH   = 8
};

enum class HttpResponse : int {
  Continue                    = 100,
  SwitchingProtocols          = 101,
  EarlyHints                  = 103,
  Ok                          = 200,
  Created                     = 201,
  Accepted                    = 202,
  NonAuthoritativeInformation = 203,
  NoContent                   = 204,
  ResetContent                = 205,
  PartialContent              = 206,
  MultipleChoices             = 300,
  MovedPermanently            = 301,
  Found                       = 302,
  NotModified                 = 304,
  BadRequest                  = 400,
  Unauthorized                = 401,
  Forbidden                   = 403,
  NotFound                    = 404,
  MethodNotAllowed            = 405,
  RequestTimeout              = 408,
  ImATeapot                   = 418,
  InternalServerError         = 500,
  NotImplemented              = 501,
  BadGateway                  = 502,
  ServiceUnvailable           = 503,
  GatewayTimeout              = 504,
  HttpVersionNotSupported     = 505
};

}

#endif