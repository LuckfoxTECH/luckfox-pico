# IPC RESTful API Guide

---

## JSON Code Style

Follow [Google JSON Style Guide](https://google.github.io/styleguide/jsoncstyleguide.xml)

## Basic API Flow

Method Check : GET, POST
URI Check :  api/v1/type?param=value&&key=token
  API Version Match ?
  Type Exists ?
  Params Decode
  Authentication ?
Header Check :
  Common : Content-Type, Authentication
  Auth ?
  Expires ?
  Content Type Valid ? Length ? (POST ONLY)
Parse Content (POST ONLY)
  JSON Parse Success ?
  API Version Match ?
  data exists ? kind exists ?
Specific Handler by method && type && kind :
  Parse item ? param ? (POST ONLY)
  return item by param

## Authentication

### Login(Clear Text By Now)

> POST login?name=admin&&password=password

Request:

```txt
Content-Type: application/json

{
  "password":"password",
  "username":"admin"
}
```

Response:

```txt
HTTP/1.1 200 OK
Content-Type: application/json;charset-utf8

{
  "success": true,
  "message": "Authentication successful!",
  "token": "eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJ1c2VybmFtZSI6ImFkbWluIiwiaWF0IjoxNTM0OTMzNTY2LCJleHAiOjE1MzUwMTk5NjZ9.3xOdoxpK8hb42ykjMIl6rwLafB63Y-EQNOO9fFamp68",
  "type": Bearer,
  "expires": 3600
}
```

### Request with Token

> GET api/v1/network?type=wifi&&key=[YOUR ACCESS TOKEN]

Request Header:

```text
Vary: Origin, X-Origin
Content-Type: application/json;charset-utf8
Authorization: Bearer eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXUyJ9.eyJpc3MiOiJhdXRoMCJ9.AbIJTDMFc7yUa5MhvcP03nJPyCPzZtQcGEp-zWfOkEEeyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJ1c2VybmFtZSI6ImFkbWluIiwiaWF0IjoxNTM0OTMzNTY2LCJleHAiOjE1MzUwMTk5NjZ9.3xOdoxpK8hb42ykjMIl6rwLafB63Y-EQNOO9fFamp68

{}
```

Response on success:

```text
Content-Type: application/json;charset-utf8

{
  "apiVersion": 1.0,
  "kind": "network#wifi",
  "data": {
    "updated": "2019-10-29T19:29:54.001Z",
    "items": [
      {
        "service": "wifi_8cf710c4d079_54502d4c494e4b5f48_managed_psk",
        "ipv4": {
          "Method": "dhcp",
          "Address": "192.168.123.123",
          "Gateway": "192.168.123.1",
          "Netmask": "255.255.255.0"
        },
        "Ethernet": {
          "Method": "auto",
          "Interface": "wlan0",
          "Address": "8C:F7:10:C4:D0:79"
        },
        "Nameservers": [
          "1.1.1.1",
          "8.8.8.8"
        ]
      },
      {
        "service": "wifi_8cf710c4d079_574946495f54455354_managed_psk",
        "Nameservers": "",
        "Security": "psk",
        "ipv4": {
          "Method": "",
          "Address": "",
          "Netmask": "",
          "Gateway": ""
        },
        "ethernet": {
          "Method": "auto",
          "Interface": "wlan0",
          "Address": "8C:F7:10:C4:D0:79"
        }
      }
    ]
  }
}
```

Response on error:

```text
HTTP/1.1 400 Bad Request
Content-Type: application/json;charset-utf8

{
  error: {
    code: 400,
    message: "Auth token invalid!"
  }
}
```
