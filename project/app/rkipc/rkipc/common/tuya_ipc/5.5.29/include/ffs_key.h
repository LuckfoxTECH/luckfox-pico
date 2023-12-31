#ifndef __FFS_KEY_H__
#define __FFS_KEY_H__
#if TUYA_FFS_TEST_MODE

//#define WIPRO_8
//#define IPC_1
#define COMMON_THBG23JMC_1
#ifdef COMMON_THBG23JMC_1

#define FFS_PID_TEST ("RLM2")
#define FFS_PIN_TEST ("QuMCv58GG")
#define FFS_SERIAL_NUMBER_TEST ("THBG23JMC00001")

const CHAR_T *cloud_public_key =
    "-----BEGIN PUBLIC KEY-----\r\n"
    "MFkwEwYHKoZIzj0CAQYIKoZIzj0DAQcDQgAE7qIHbKAKLmYo2IFuZ391KWXLhY51\r\n"
    "KeLCfxxaUGRQ2+Ipuey9raXWS+BxscQRtqw242fEs9kfaRYT+t+tAeQjaw==\r\n"
    "-----END PUBLIC KEY-----\r\n";

const CHAR_T *dev_private_key =
    "-----BEGIN PRIVATE KEY-----\r\n"
    "MIGHAgEAMBMGByqGSM49AgEGCCqGSM49AwEHBG0wawIBAQQgww1aKS8dQv/CiiY9\r\n"
    "H2WvFBAibCkvjRqEwMDPoa4xwRGhRANCAARlskqMIPfYfSH4MW9Jk7dId/AqJt1K\r\n"
    "6jqs82LR9KKOGxcPQS1A2U9tcKNBgwDnrWIyXcOhrpbYUEFA1r80Nuum\r\n"
    "-----END PRIVATE KEY-----";

const CHAR_T amazon_cert[] = "-----BEGIN CERTIFICATE-----\r\n"
                             "MIIB7zCCAZWgAwIBAgIEKuP5mzAKBggqhkjOPQQDAjCBjjELMAkGA1UEBhMCVVMx\r\n"
                             "EzARBgNVBAgMCldhc2hpbmd0b24xEDAOBgNVBAcMB1NlYXR0bGUxGDAWBgNVBAoM\r\n"
                             "D0FtYXpvbi5jb20gSW5jLjEaMBgGA1UECwwRRGV2aWNlIE1hbmFnZW1lbnQxIjAg\r\n"
                             "BgNVBAMMGURBSyBDQSBmb3IgQTNHN0w4Wk9QQ01MU1owHhcNMjAwMzE2MDI0MjEz\r\n"
                             "WhcNNDAwMzE2MDI0MjEzWjAfMR0wGwYJKwYBBAGlawEDDA5BM0c3TDhaT1BDTUxT\r\n"
                             "WjBZMBMGByqGSM49AgEGCCqGSM49AwEHA0IABGWySowg99h9Ifgxb0mTt0h38Com\r\n"
                             "3UrqOqzzYtH0oo4bFw9BLUDZT21wo0GDAOetYjJdw6GulthQQUDWvzQ266ajTzBN\r\n"
                             "MB0GA1UdDgQWBBQhGYzuj4qc7zzGJgThX70lIEygPDAfBgNVHSMEGDAWgBRt/d4w\r\n"
                             "tk20QwCrH3vpt47oCpw6tzALBgNVHQ8EBAMCA6gwCgYIKoZIzj0EAwIDSAAwRQIh\r\n"
                             "AJYysHdXcLBYkbkF7JV84t4N4IWXNUvNcreU9UDPRcHtAiBga2SXBiNIgkrM68gm\r\n"
                             "jsGFHiLFMvNftMbDHa9MKCnIsQ==\r\n"
                             "-----END CERTIFICATE-----\r\n"
                             "-----BEGIN CERTIFICATE-----\r\n"
                             "MIIDVDCCAvqgAwIBAgIOAMbo00n8Whivijn5xjAwCgYIKoZIzj0EAwIwgaYxCzAJ\r\n"
                             "BgNVBAYTAlVTMRMwEQYDVQQIDApXYXNoaW5ndG9uMRAwDgYDVQQHDAdTZWF0dGxl\r\n"
                             "MRgwFgYDVQQKDA9BbWF6b24uY29tIEluYy4xGjAYBgNVBAsMEURldmljZSBNYW5h\r\n"
                             "Z2VtZW50MTowOAYDVQQDDDFBbWF6b25ESEEgVHV5YV9Qcm9kdWN0X0EzRzdMOFpP\r\n"
                             "UENNTFNaIENlcnRpZmljYXRlMB4XDTE5MTIwOTIwMjY1M1oXDTM5MTIwOTIwMjY1\r\n"
                             "M1owgY4xCzAJBgNVBAYTAlVTMRMwEQYDVQQIDApXYXNoaW5ndG9uMRAwDgYDVQQH\r\n"
                             "DAdTZWF0dGxlMRgwFgYDVQQKDA9BbWF6b24uY29tIEluYy4xGjAYBgNVBAsMEURl\r\n"
                             "dmljZSBNYW5hZ2VtZW50MSIwIAYDVQQDDBlEQUsgQ0EgZm9yIEEzRzdMOFpPUENN\r\n"
                             "TFNaMFkwEwYHKoZIzj0CAQYIKoZIzj0DAQcDQgAE2mnT36IJ8nhXXVPbru0NjlGn\r\n"
                             "uYglVNVhCJSjJ5tBuf7Yeyzy6Kpqt58kAu/YQUNgahNHbnpBbPcORb017+4FKaOC\r\n"
                             "ASAwggEcMBIGA1UdEwEB/wQIMAYBAf8CAQAwHQYDVR0OBBYEFG393jC2TbRDAKsf\r\n"
                             "e+m3jugKnDq3MIHWBgNVHSMEgc4wgcuAFJicBJERH1rDqspSeVE+d5L5Qu5EoYGi\r\n"
                             "pIGfMIGcMQswCQYDVQQGEwJVUzETMBEGA1UECAwKV2FzaGluZ3RvbjEQMA4GA1UE\r\n"
                             "BwwHU2VhdHRsZTEYMBYGA1UECgwPQW1hem9uLmNvbSBJbmMuMRowGAYDVQQLDBFE\r\n"
                             "ZXZpY2UgTWFuYWdlbWVudDEwMC4GA1UEAwwnQW1hem9uREhBIFR1eWFfTWFudWZh\r\n"
                             "Y3R1cmVyIENlcnRpZmljYXRlgg4AxujSxL+8WFgMkdK2mzAOBgNVHQ8BAf8EBAMC\r\n"
                             "AoQwCgYIKoZIzj0EAwIDSAAwRQIhANhJdpDuHVHXE3DJo7WYTHyECsANwrcYA9CD\r\n"
                             "gvGQXsmBAiB9nejkOcBlDJaMUkLyT12go0IVn4Bcxs85uIDJb5vrGA==\r\n"
                             "-----END CERTIFICATE-----\r\n"
                             "-----BEGIN CERTIFICATE-----\r\n"
                             "MIIDVjCCAv2gAwIBAgIOAMbo0sS/vFhYDJHStpswCgYIKoZIzj0EAwIwgZwxCzAJ\r\n"
                             "BgNVBAYTAlVTMRMwEQYDVQQIDApXYXNoaW5ndG9uMRAwDgYDVQQHDAdTZWF0dGxl\r\n"
                             "MRgwFgYDVQQKDA9BbWF6b24uY29tIEluYy4xGjAYBgNVBAsMEURldmljZSBNYW5h\r\n"
                             "Z2VtZW50MTAwLgYDVQQDDCdBbWF6b25ESEEgVHV5YV9NYW51ZmFjdHVyZXIgQ2Vy\r\n"
                             "dGlmaWNhdGUwHhcNMTkxMjA5MjAyNTUwWhcNMzkxMjA5MjAyNTUwWjCBpjELMAkG\r\n"
                             "A1UEBhMCVVMxEzARBgNVBAgMCldhc2hpbmd0b24xEDAOBgNVBAcMB1NlYXR0bGUx\r\n"
                             "GDAWBgNVBAoMD0FtYXpvbi5jb20gSW5jLjEaMBgGA1UECwwRRGV2aWNlIE1hbmFn\r\n"
                             "ZW1lbnQxOjA4BgNVBAMMMUFtYXpvbkRIQSBUdXlhX1Byb2R1Y3RfQTNHN0w4Wk9Q\r\n"
                             "Q01MU1ogQ2VydGlmaWNhdGUwWTATBgcqhkjOPQIBBggqhkjOPQMBBwNCAAT6x5p2\r\n"
                             "gMF55IxHIhYeZYvi9JHoIgf0f5DZFhLRew/4cyvp35/gg4qmS5TyJj/w0/kRlWlQ\r\n"
                             "92xqUSXCmV5Z7mz2o4IBFTCCAREwDwYDVR0TAQH/BAUwAwEB/zAdBgNVHQ4EFgQU\r\n"
                             "mJwEkREfWsOqylJ5UT53kvlC7kQwgc4GA1UdIwSBxjCBw4AU8RzI5oQ+OsSvsUub\r\n"
                             "ORRuOEETp/ShgZqkgZcwgZQxFDASBgNVBAoMC0FtYXpvbiBJbmMuMSMwIQYDVQQL\r\n"
                             "DBpEZXZpY2UgTWFuYWdlbWVudCBTZXJ2aWNlczEQMA4GA1UEBwwHU2VhdHRsZTET\r\n"
                             "MBEGA1UECAwKV2FzaGluZ3RvbjELMAkGA1UEBhMCVVMxIzAhBgNVBAMMGkFtYXpv\r\n"
                             "bkRIQSBSb290IENlcnRpZmljYXRlgg4AxujSrs5VP8cNqAB3ZjAOBgNVHQ8BAf8E\r\n"
                             "BAMCAYYwCgYIKoZIzj0EAwIDRwAwRAIgIUDhICIQzclWoZXHGFMIkFajYIO0DcYk\r\n"
                             "lFJVx5d+uHACIGy3fKfqenETqIH5snJsFR7QnEQ47Qq3K9ceGh2Azz1m\r\n"
                             "-----END CERTIFICATE-----\r\n"
                             "-----BEGIN CERTIFICATE-----\r\n"
                             "MIIDQDCCAuagAwIBAgIOAMbo0q7OVT/HDagAd2YwCgYIKoZIzj0EAwIwgZQxFDAS\r\n"
                             "BgNVBAoMC0FtYXpvbiBJbmMuMSMwIQYDVQQLDBpEZXZpY2UgTWFuYWdlbWVudCBT\r\n"
                             "ZXJ2aWNlczEQMA4GA1UEBwwHU2VhdHRsZTETMBEGA1UECAwKV2FzaGluZ3RvbjEL\r\n"
                             "MAkGA1UEBhMCVVMxIzAhBgNVBAMMGkFtYXpvbkRIQSBSb290IENlcnRpZmljYXRl\r\n"
                             "MB4XDTE5MTIwOTIwMjU0MFoXDTM5MTIwOTIwMjU0MFowgZwxCzAJBgNVBAYTAlVT\r\n"
                             "MRMwEQYDVQQIDApXYXNoaW5ndG9uMRAwDgYDVQQHDAdTZWF0dGxlMRgwFgYDVQQK\r\n"
                             "DA9BbWF6b24uY29tIEluYy4xGjAYBgNVBAsMEURldmljZSBNYW5hZ2VtZW50MTAw\r\n"
                             "LgYDVQQDDCdBbWF6b25ESEEgVHV5YV9NYW51ZmFjdHVyZXIgQ2VydGlmaWNhdGUw\r\n"
                             "WTATBgcqhkjOPQIBBggqhkjOPQMBBwNCAARBmDa+6dYNG2/KBpLv08bn6e1lmoKf\r\n"
                             "1hv0e8W3tQW97u1kFpiq3BF1MaQqrAiV0vqFItWOeWCXwV7oy3hz+6Q7o4IBEDCC\r\n"
                             "AQwwDwYDVR0TAQH/BAUwAwEB/zAdBgNVHQ4EFgQU8RzI5oQ+OsSvsUubORRuOEET\r\n"
                             "p/QwgckGA1UdIwSBwTCBvoAUYYvcTyRKpW7uCmW8vHSaUZXqq2ChgZqkgZcwgZQx\r\n"
                             "FDASBgNVBAoMC0FtYXpvbiBJbmMuMSMwIQYDVQQLDBpEZXZpY2UgTWFuYWdlbWVu\r\n"
                             "dCBTZXJ2aWNlczEQMA4GA1UEBwwHU2VhdHRsZTETMBEGA1UECAwKV2FzaGluZ3Rv\r\n"
                             "bjELMAkGA1UEBhMCVVMxIzAhBgNVBAMMGkFtYXpvbkRIQSBSb290IENlcnRpZmlj\r\n"
                             "YXRlggkAqkHFg3IfxuIwDgYDVR0PAQH/BAQDAgGGMAoGCCqGSM49BAMCA0gAMEUC\r\n"
                             "IQDnRZkmmDemoO+Mk7yutOIpp0mF2Cj4+1Y35uHLnobBfQIgOuZdBZK4Pab8TD2g\r\n"
                             "9TzvlsmQkQqm1VTNyqwykH0o+5g=\r\n"
                             "-----END CERTIFICATE-----\r\n"
                             "-----BEGIN CERTIFICATE-----\r\n"
                             "MIIDNjCCAtugAwIBAgIJAKpBxYNyH8biMAoGCCqGSM49BAMCMIGUMRQwEgYDVQQK\r\n"
                             "DAtBbWF6b24gSW5jLjEjMCEGA1UECwwaRGV2aWNlIE1hbmFnZW1lbnQgU2Vydmlj\r\n"
                             "ZXMxEDAOBgNVBAcMB1NlYXR0bGUxEzARBgNVBAgMCldhc2hpbmd0b24xCzAJBgNV\r\n"
                             "BAYTAlVTMSMwIQYDVQQDDBpBbWF6b25ESEEgUm9vdCBDZXJ0aWZpY2F0ZTAgFw0x\r\n"
                             "NzExMTYwNjI3MDlaGA8yMTE3MTEwODA2MjcwOVowgZQxFDASBgNVBAoMC0FtYXpv\r\n"
                             "biBJbmMuMSMwIQYDVQQLDBpEZXZpY2UgTWFuYWdlbWVudCBTZXJ2aWNlczEQMA4G\r\n"
                             "A1UEBwwHU2VhdHRsZTETMBEGA1UECAwKV2FzaGluZ3RvbjELMAkGA1UEBhMCVVMx\r\n"
                             "IzAhBgNVBAMMGkFtYXpvbkRIQSBSb290IENlcnRpZmljYXRlMFkwEwYHKoZIzj0C\r\n"
                             "AQYIKoZIzj0DAQcDQgAEv7+Zfvkc+qvUaKgaGxQMZoDFHQ18Z5OSXB4BNkYRszgR\r\n"
                             "PQ82o54KVb0RKkkq0e3niRn1gUZ8jozePEPrpPV5yaOCARAwggEMMA8GA1UdEwEB\r\n"
                             "/wQFMAMBAf8wHQYDVR0OBBYEFGGL3E8kSqVu7gplvLx0mlGV6qtgMIHJBgNVHSME\r\n"
                             "gcEwgb6AFGGL3E8kSqVu7gplvLx0mlGV6qtgoYGapIGXMIGUMRQwEgYDVQQKDAtB\r\n"
                             "bWF6b24gSW5jLjEjMCEGA1UECwwaRGV2aWNlIE1hbmFnZW1lbnQgU2VydmljZXMx\r\n"
                             "EDAOBgNVBAcMB1NlYXR0bGUxEzARBgNVBAgMCldhc2hpbmd0b24xCzAJBgNVBAYT\r\n"
                             "AlVTMSMwIQYDVQQDDBpBbWF6b25ESEEgUm9vdCBDZXJ0aWZpY2F0ZYIJAKpBxYNy\r\n"
                             "H8biMA4GA1UdDwEB/wQEAwIBhjAKBggqhkjOPQQDAgNJADBGAiEAoCM4t1cMuTeu\r\n"
                             "8yIlw/1BUIUb1Q4MYXp+LyfjcbmVz8ECIQDuKW8gQZjUS8Z7GcgNYnFup3UjTznj\r\n"
                             "5ja4/PmvDx0Glw==\r\n"
                             "-----END CERTIFICATE-----";

#endif
#ifdef IPC_1

#define FFS_PID_TEST ("yczP")
#define FFS_PIN_TEST ("fYM0q0Ru0")
#define FFS_SERIAL_NUMBER_TEST ("GEBF12MRP00001")

const CHAR_T *cloud_public_key =
    "-----BEGIN PUBLIC KEY-----\r\n"
    "MFkwEwYHKoZIzj0CAQYIKoZIzj0DAQcDQgAE7qIHbKAKLmYo2IFuZ391KWXLhY51\r\n"
    "KeLCfxxaUGRQ2+Ipuey9raXWS+BxscQRtqw242fEs9kfaRYT+t+tAeQjaw==\r\n"
    "-----END PUBLIC KEY-----\r\n";

const CHAR_T *dev_private_key =
    "-----BEGIN PRIVATE KEY-----\r\n"
    "MIGHAgEAMBMGByqGSM49AgEGCCqGSM49AwEHBG0wawIBAQQgSmg4ZmkOnnfbkr6e\r\n"
    "J9olSiOQcI3In90o7Z5mc5jBNNKhRANCAARI/h6GgB26LzS91sy44FOL1Xk3Uw28\r\n"
    "e6DKlLtz57PH6qT1HriuQbsqyJUGtwaNS2GuTjBRtUfU4pxtR3x8QvAp\r\n"
    "-----END PRIVATE KEY-----";

const CHAR_T amazon_cert[] = "-----BEGIN CERTIFICATE-----\r\n"
                             "MIIB7zCCAZWgAwIBAgIEf5abBDAKBggqhkjOPQQDAjCBjjELMAkGA1UEBhMCVVMx\r\n"
                             "EzARBgNVBAgMCldhc2hpbmd0b24xEDAOBgNVBAcMB1NlYXR0bGUxGDAWBgNVBAoM\r\n"
                             "D0FtYXpvbi5jb20gSW5jLjEaMBgGA1UECwwRRGV2aWNlIE1hbmFnZW1lbnQxIjAg\r\n"
                             "BgNVBAMMGURBSyBDQSBmb3IgQTEwMU5VVlZOUzc2MzQwHhcNMjAwNTIxMDQwODIy\r\n"
                             "WhcNNDAwNTIxMDQwODIyWjAfMR0wGwYJKwYBBAGlawEDDA5BMTAxTlVWVk5TNzYz\r\n"
                             "NDBZMBMGByqGSM49AgEGCCqGSM49AwEHA0IABEj+HoaAHbovNL3WzLjgU4vVeTdT\r\n"
                             "Dbx7oMqUu3Pns8fqpPUeuK5BuyrIlQa3Bo1LYa5OMFG1R9TinG1HfHxC8CmjTzBN\r\n"
                             "MB0GA1UdDgQWBBRXoaVEB5rcaYsf3zF/SIesWIoHxTAfBgNVHSMEGDAWgBTPYP0y\r\n"
                             "cVxp8p67ybjjw1nHIeXPzjALBgNVHQ8EBAMCA6gwCgYIKoZIzj0EAwIDSAAwRQIh\r\n"
                             "AIEuXNXw+A8RIJrbGIFf1TzlQ0/mYplWKwFuC27jpWdGAiAInPm8uB9QaTM3Nr8z\r\n"
                             "zp4O5vZQrCYMRFcvUreNIglTrw==\r\n"
                             "-----END CERTIFICATE-----\r\n"
                             "-----BEGIN CERTIFICATE-----\r\n"
                             "MIIDVDCCAvqgAwIBAgIOAMh0yNgkaQl0BGq/jbUwCgYIKoZIzj0EAwIwgaYxCzAJ\r\n"
                             "BgNVBAYTAlVTMRMwEQYDVQQIDApXYXNoaW5ndG9uMRAwDgYDVQQHDAdTZWF0dGxl\r\n"
                             "MRgwFgYDVQQKDA9BbWF6b24uY29tIEluYy4xGjAYBgNVBAsMEURldmljZSBNYW5h\r\n"
                             "Z2VtZW50MTowOAYDVQQDDDFBbWF6b25ESEEgdHV5YV9Qcm9kdWN0X0ExMDFOVVZW\r\n"
                             "TlM3NjM0IENlcnRpZmljYXRlMB4XDTIwMDQyOTE2MjU1NloXDTQwMDQyOTE2MjU1\r\n"
                             "NlowgY4xCzAJBgNVBAYTAlVTMRMwEQYDVQQIDApXYXNoaW5ndG9uMRAwDgYDVQQH\r\n"
                             "DAdTZWF0dGxlMRgwFgYDVQQKDA9BbWF6b24uY29tIEluYy4xGjAYBgNVBAsMEURl\r\n"
                             "dmljZSBNYW5hZ2VtZW50MSIwIAYDVQQDDBlEQUsgQ0EgZm9yIEExMDFOVVZWTlM3\r\n"
                             "NjM0MFkwEwYHKoZIzj0CAQYIKoZIzj0DAQcDQgAEiD3cM+GTfNsfcJSO1zHbafKe\r\n"
                             "MbdsHCU5vHzdD3IURcteJgnvQqjSMce/A82EmfL8G6GcHgq63j1Cik7dYPL5qKOC\r\n"
                             "ASAwggEcMBIGA1UdEwEB/wQIMAYBAf8CAQAwHQYDVR0OBBYEFM9g/TJxXGnynrvJ\r\n"
                             "uOPDWcch5c/OMIHWBgNVHSMEgc4wgcuAFBKrjoX+A7DTDyRzm5td9WGz91TYoYGi\r\n"
                             "pIGfMIGcMQswCQYDVQQGEwJVUzETMBEGA1UECAwKV2FzaGluZ3RvbjEQMA4GA1UE\r\n"
                             "BwwHU2VhdHRsZTEYMBYGA1UECgwPQW1hem9uLmNvbSBJbmMuMRowGAYDVQQLDBFE\r\n"
                             "ZXZpY2UgTWFuYWdlbWVudDEwMC4GA1UEAwwnQW1hem9uREhBIFR1eWFfTWFudWZh\r\n"
                             "Y3R1cmVyIENlcnRpZmljYXRlgg4AyHTIzjPtVZWdZb0jGDAOBgNVHQ8BAf8EBAMC\r\n"
                             "AoQwCgYIKoZIzj0EAwIDSAAwRQIgBA7X5ncLg8Uwht+segzN0zSi2AwDpcOWIa4D\r\n"
                             "9DcA3WoCIQCgsqkBjYr60zfyF0irOgJrlh+HgulKudfc/ESTSJs0aQ==\r\n"
                             "-----END CERTIFICATE-----\r\n"
                             "-----BEGIN CERTIFICATE-----\r\n"
                             "MIIDWDCCAv2gAwIBAgIOAMh0yM4z7VWVnWW9IxgwCgYIKoZIzj0EAwIwgZwxCzAJ\r\n"
                             "BgNVBAYTAlVTMRMwEQYDVQQIDApXYXNoaW5ndG9uMRAwDgYDVQQHDAdTZWF0dGxl\r\n"
                             "MRgwFgYDVQQKDA9BbWF6b24uY29tIEluYy4xGjAYBgNVBAsMEURldmljZSBNYW5h\r\n"
                             "Z2VtZW50MTAwLgYDVQQDDCdBbWF6b25ESEEgVHV5YV9NYW51ZmFjdHVyZXIgQ2Vy\r\n"
                             "dGlmaWNhdGUwHhcNMjAwNDI5MTYyNTUyWhcNNDAwNDI5MTYyNTUyWjCBpjELMAkG\r\n"
                             "A1UEBhMCVVMxEzARBgNVBAgMCldhc2hpbmd0b24xEDAOBgNVBAcMB1NlYXR0bGUx\r\n"
                             "GDAWBgNVBAoMD0FtYXpvbi5jb20gSW5jLjEaMBgGA1UECwwRRGV2aWNlIE1hbmFn\r\n"
                             "ZW1lbnQxOjA4BgNVBAMMMUFtYXpvbkRIQSB0dXlhX1Byb2R1Y3RfQTEwMU5VVlZO\r\n"
                             "Uzc2MzQgQ2VydGlmaWNhdGUwWTATBgcqhkjOPQIBBggqhkjOPQMBBwNCAAQPXnAv\r\n"
                             "MfkLIPa36y1tytRiQnM6EML+KAS4yuDjN+WiqsjzAbmmrJISd0FOdaiKZLRy1CqQ\r\n"
                             "8LPlnLeWN+ez75N6o4IBFTCCAREwDwYDVR0TAQH/BAUwAwEB/zAdBgNVHQ4EFgQU\r\n"
                             "EquOhf4DsNMPJHObm131YbP3VNgwgc4GA1UdIwSBxjCBw4AU8RzI5oQ+OsSvsUub\r\n"
                             "ORRuOEETp/ShgZqkgZcwgZQxFDASBgNVBAoMC0FtYXpvbiBJbmMuMSMwIQYDVQQL\r\n"
                             "DBpEZXZpY2UgTWFuYWdlbWVudCBTZXJ2aWNlczEQMA4GA1UEBwwHU2VhdHRsZTET\r\n"
                             "MBEGA1UECAwKV2FzaGluZ3RvbjELMAkGA1UEBhMCVVMxIzAhBgNVBAMMGkFtYXpv\r\n"
                             "bkRIQSBSb290IENlcnRpZmljYXRlgg4AxujSrs5VP8cNqAB3ZjAOBgNVHQ8BAf8E\r\n"
                             "BAMCAYYwCgYIKoZIzj0EAwIDSQAwRgIhALak4BjOMpaXW+ZnrMnfJQMfhyfDCw4r\r\n"
                             "n2k6AlIJOb6KAiEAtZd1Xbp/qn7h16tosTdbeJCO9AYBURUDpiR1f6tKI4Q=\r\n"
                             "-----END CERTIFICATE-----\r\n"
                             "-----BEGIN CERTIFICATE-----\r\n"
                             "MIIDQDCCAuagAwIBAgIOAMbo0q7OVT/HDagAd2YwCgYIKoZIzj0EAwIwgZQxFDAS\r\n"
                             "BgNVBAoMC0FtYXpvbiBJbmMuMSMwIQYDVQQLDBpEZXZpY2UgTWFuYWdlbWVudCBT\r\n"
                             "ZXJ2aWNlczEQMA4GA1UEBwwHU2VhdHRsZTETMBEGA1UECAwKV2FzaGluZ3RvbjEL\r\n"
                             "MAkGA1UEBhMCVVMxIzAhBgNVBAMMGkFtYXpvbkRIQSBSb290IENlcnRpZmljYXRl\r\n"
                             "MB4XDTE5MTIwOTIwMjU0MFoXDTM5MTIwOTIwMjU0MFowgZwxCzAJBgNVBAYTAlVT\r\n"
                             "MRMwEQYDVQQIDApXYXNoaW5ndG9uMRAwDgYDVQQHDAdTZWF0dGxlMRgwFgYDVQQK\r\n"
                             "DA9BbWF6b24uY29tIEluYy4xGjAYBgNVBAsMEURldmljZSBNYW5hZ2VtZW50MTAw\r\n"
                             "LgYDVQQDDCdBbWF6b25ESEEgVHV5YV9NYW51ZmFjdHVyZXIgQ2VydGlmaWNhdGUw\r\n"
                             "WTATBgcqhkjOPQIBBggqhkjOPQMBBwNCAARBmDa+6dYNG2/KBpLv08bn6e1lmoKf\r\n"
                             "1hv0e8W3tQW97u1kFpiq3BF1MaQqrAiV0vqFItWOeWCXwV7oy3hz+6Q7o4IBEDCC\r\n"
                             "AQwwDwYDVR0TAQH/BAUwAwEB/zAdBgNVHQ4EFgQU8RzI5oQ+OsSvsUubORRuOEET\r\n"
                             "p/QwgckGA1UdIwSBwTCBvoAUYYvcTyRKpW7uCmW8vHSaUZXqq2ChgZqkgZcwgZQx\r\n"
                             "FDASBgNVBAoMC0FtYXpvbiBJbmMuMSMwIQYDVQQLDBpEZXZpY2UgTWFuYWdlbWVu\r\n"
                             "dCBTZXJ2aWNlczEQMA4GA1UEBwwHU2VhdHRsZTETMBEGA1UECAwKV2FzaGluZ3Rv\r\n"
                             "bjELMAkGA1UEBhMCVVMxIzAhBgNVBAMMGkFtYXpvbkRIQSBSb290IENlcnRpZmlj\r\n"
                             "YXRlggkAqkHFg3IfxuIwDgYDVR0PAQH/BAQDAgGGMAoGCCqGSM49BAMCA0gAMEUC\r\n"
                             "IQDnRZkmmDemoO+Mk7yutOIpp0mF2Cj4+1Y35uHLnobBfQIgOuZdBZK4Pab8TD2g\r\n"
                             "9TzvlsmQkQqm1VTNyqwykH0o+5g=\r\n"
                             "-----END CERTIFICATE-----\r\n"
                             "-----BEGIN CERTIFICATE-----\r\n"
                             "MIIDNjCCAtugAwIBAgIJAKpBxYNyH8biMAoGCCqGSM49BAMCMIGUMRQwEgYDVQQK\r\n"
                             "DAtBbWF6b24gSW5jLjEjMCEGA1UECwwaRGV2aWNlIE1hbmFnZW1lbnQgU2Vydmlj\r\n"
                             "ZXMxEDAOBgNVBAcMB1NlYXR0bGUxEzARBgNVBAgMCldhc2hpbmd0b24xCzAJBgNV\r\n"
                             "BAYTAlVTMSMwIQYDVQQDDBpBbWF6b25ESEEgUm9vdCBDZXJ0aWZpY2F0ZTAgFw0x\r\n"
                             "NzExMTYwNjI3MDlaGA8yMTE3MTEwODA2MjcwOVowgZQxFDASBgNVBAoMC0FtYXpv\r\n"
                             "biBJbmMuMSMwIQYDVQQLDBpEZXZpY2UgTWFuYWdlbWVudCBTZXJ2aWNlczEQMA4G\r\n"
                             "A1UEBwwHU2VhdHRsZTETMBEGA1UECAwKV2FzaGluZ3RvbjELMAkGA1UEBhMCVVMx\r\n"
                             "IzAhBgNVBAMMGkFtYXpvbkRIQSBSb290IENlcnRpZmljYXRlMFkwEwYHKoZIzj0C\r\n"
                             "AQYIKoZIzj0DAQcDQgAEv7+Zfvkc+qvUaKgaGxQMZoDFHQ18Z5OSXB4BNkYRszgR\r\n"
                             "PQ82o54KVb0RKkkq0e3niRn1gUZ8jozePEPrpPV5yaOCARAwggEMMA8GA1UdEwEB\r\n"
                             "/wQFMAMBAf8wHQYDVR0OBBYEFGGL3E8kSqVu7gplvLx0mlGV6qtgMIHJBgNVHSME\r\n"
                             "gcEwgb6AFGGL3E8kSqVu7gplvLx0mlGV6qtgoYGapIGXMIGUMRQwEgYDVQQKDAtB\r\n"
                             "bWF6b24gSW5jLjEjMCEGA1UECwwaRGV2aWNlIE1hbmFnZW1lbnQgU2VydmljZXMx\r\n"
                             "EDAOBgNVBAcMB1NlYXR0bGUxEzARBgNVBAgMCldhc2hpbmd0b24xCzAJBgNVBAYT\r\n"
                             "AlVTMSMwIQYDVQQDDBpBbWF6b25ESEEgUm9vdCBDZXJ0aWZpY2F0ZYIJAKpBxYNy\r\n"
                             "H8biMA4GA1UdDwEB/wQEAwIBhjAKBggqhkjOPQQDAgNJADBGAiEAoCM4t1cMuTeu\r\n"
                             "8yIlw/1BUIUb1Q4MYXp+LyfjcbmVz8ECIQDuKW8gQZjUS8Z7GcgNYnFup3UjTznj\r\n"
                             "5ja4/PmvDx0Glw==\r\n"
                             "-----END CERTIFICATE-----";
#endif

#ifdef WIPRO_8

// wipro #8 ok
#define FFS_PID_TEST ("RLM2")
#define FFS_PIN_TEST ("TvS5w66KL")
#define FFS_SERIAL_NUMBER_TEST ("SLBE25APT00008")

const CHAR_T *cloud_public_key =
    "-----BEGIN PUBLIC KEY-----\r\n"
    "MFkwEwYHKoZIzj0CAQYIKoZIzj0DAQcDQgAE7qIHbKAKLmYo2IFuZ391KWXLhY51\r\n"
    "KeLCfxxaUGRQ2+Ipuey9raXWS+BxscQRtqw242fEs9kfaRYT+t+tAeQjaw==\r\n"
    "-----END PUBLIC KEY-----\r\n";

const CHAR_T *dev_private_key =
    "-----BEGIN PRIVATE KEY-----\r\n"
    "MIGHAgEAMBMGByqGSM49AgEGCCqGSM49AwEHBG0wawIBAQQgoGpApBXm4Z+W3EU0\r\n"
    "s39nwC2QZTG7S6aBKNzH+HCqv+ShRANCAARpsjgot5sLEfRBuH7KxMhpX37LeABd\r\n"
    "I0lT/T+vi+rEnqI2JKNKNrCVWJ9EIb0nUHBxXEcjG/wO+J7doGrqc6Xb\r\n"
    "-----END PRIVATE KEY-----";

const CHAR_T amazon_cert[] = "-----BEGIN CERTIFICATE-----\r\n"
                             "MIIB7jCCAZWgAwIBAgIES7/b+jAKBggqhkjOPQQDAjCBjjELMAkGA1UEBhMCVVMx\r\n"
                             "EzARBgNVBAgMCldhc2hpbmd0b24xEDAOBgNVBAcMB1NlYXR0bGUxGDAWBgNVBAoM\r\n"
                             "D0FtYXpvbi5jb20gSW5jLjEaMBgGA1UECwwRRGV2aWNlIE1hbmFnZW1lbnQxIjAg\r\n"
                             "BgNVBAMMGURBSyBDQSBmb3IgQTNHN0w4Wk9QQ01MU1owHhcNMjAwMzE2MDI0MTQ5\r\n"
                             "WhcNNDAwMzE2MDI0MTQ5WjAfMR0wGwYJKwYBBAGlawEDDA5BM0c3TDhaT1BDTUxT\r\n"
                             "WjBZMBMGByqGSM49AgEGCCqGSM49AwEHA0IABGmyOCi3mwsR9EG4fsrEyGlffst4\r\n"
                             "AF0jSVP9P6+L6sSeojYko0o2sJVYn0QhvSdQcHFcRyMb/A74nt2gaupzpdujTzBN\r\n"
                             "MB0GA1UdDgQWBBRS97MOyh37ZBeBpkVhoyXkLTiErDAfBgNVHSMEGDAWgBRt/d4w\r\n"
                             "tk20QwCrH3vpt47oCpw6tzALBgNVHQ8EBAMCA6gwCgYIKoZIzj0EAwIDRwAwRAIg\r\n"
                             "BEpjMv3xJHDnqYbK0LDkGWJIQcML3phDjylDyhuLVV4CIANnU/Y+bYdz+KKWIbVS\r\n"
                             "rPJamALOzWPJdaoSzNnai2Ph\r\n"
                             "-----END CERTIFICATE-----\r\n"
                             "-----BEGIN CERTIFICATE-----\r\n"
                             "MIIDVDCCAvqgAwIBAgIOAMbo00n8Whivijn5xjAwCgYIKoZIzj0EAwIwgaYxCzAJ\r\n"
                             "BgNVBAYTAlVTMRMwEQYDVQQIDApXYXNoaW5ndG9uMRAwDgYDVQQHDAdTZWF0dGxl\r\n"
                             "MRgwFgYDVQQKDA9BbWF6b24uY29tIEluYy4xGjAYBgNVBAsMEURldmljZSBNYW5h\r\n"
                             "Z2VtZW50MTowOAYDVQQDDDFBbWF6b25ESEEgVHV5YV9Qcm9kdWN0X0EzRzdMOFpP\r\n"
                             "UENNTFNaIENlcnRpZmljYXRlMB4XDTE5MTIwOTIwMjY1M1oXDTM5MTIwOTIwMjY1\r\n"
                             "M1owgY4xCzAJBgNVBAYTAlVTMRMwEQYDVQQIDApXYXNoaW5ndG9uMRAwDgYDVQQH\r\n"
                             "DAdTZWF0dGxlMRgwFgYDVQQKDA9BbWF6b24uY29tIEluYy4xGjAYBgNVBAsMEURl\r\n"
                             "dmljZSBNYW5hZ2VtZW50MSIwIAYDVQQDDBlEQUsgQ0EgZm9yIEEzRzdMOFpPUENN\r\n"
                             "TFNaMFkwEwYHKoZIzj0CAQYIKoZIzj0DAQcDQgAE2mnT36IJ8nhXXVPbru0NjlGn\r\n"
                             "uYglVNVhCJSjJ5tBuf7Yeyzy6Kpqt58kAu/YQUNgahNHbnpBbPcORb017+4FKaOC\r\n"
                             "ASAwggEcMBIGA1UdEwEB/wQIMAYBAf8CAQAwHQYDVR0OBBYEFG393jC2TbRDAKsf\r\n"
                             "e+m3jugKnDq3MIHWBgNVHSMEgc4wgcuAFJicBJERH1rDqspSeVE+d5L5Qu5EoYGi\r\n"
                             "pIGfMIGcMQswCQYDVQQGEwJVUzETMBEGA1UECAwKV2FzaGluZ3RvbjEQMA4GA1UE\r\n"
                             "BwwHU2VhdHRsZTEYMBYGA1UECgwPQW1hem9uLmNvbSBJbmMuMRowGAYDVQQLDBFE\r\n"
                             "ZXZpY2UgTWFuYWdlbWVudDEwMC4GA1UEAwwnQW1hem9uREhBIFR1eWFfTWFudWZh\r\n"
                             "Y3R1cmVyIENlcnRpZmljYXRlgg4AxujSxL+8WFgMkdK2mzAOBgNVHQ8BAf8EBAMC\r\n"
                             "AoQwCgYIKoZIzj0EAwIDSAAwRQIhANhJdpDuHVHXE3DJo7WYTHyECsANwrcYA9CD\r\n"
                             "gvGQXsmBAiB9nejkOcBlDJaMUkLyT12go0IVn4Bcxs85uIDJb5vrGA==\r\n"
                             "-----END CERTIFICATE-----\r\n"
                             "-----BEGIN CERTIFICATE-----\r\n"
                             "MIIDVjCCAv2gAwIBAgIOAMbo0sS/vFhYDJHStpswCgYIKoZIzj0EAwIwgZwxCzAJ\r\n"
                             "BgNVBAYTAlVTMRMwEQYDVQQIDApXYXNoaW5ndG9uMRAwDgYDVQQHDAdTZWF0dGxl\r\n"
                             "MRgwFgYDVQQKDA9BbWF6b24uY29tIEluYy4xGjAYBgNVBAsMEURldmljZSBNYW5h\r\n"
                             "Z2VtZW50MTAwLgYDVQQDDCdBbWF6b25ESEEgVHV5YV9NYW51ZmFjdHVyZXIgQ2Vy\r\n"
                             "dGlmaWNhdGUwHhcNMTkxMjA5MjAyNTUwWhcNMzkxMjA5MjAyNTUwWjCBpjELMAkG\r\n"
                             "A1UEBhMCVVMxEzARBgNVBAgMCldhc2hpbmd0b24xEDAOBgNVBAcMB1NlYXR0bGUx\r\n"
                             "GDAWBgNVBAoMD0FtYXpvbi5jb20gSW5jLjEaMBgGA1UECwwRRGV2aWNlIE1hbmFn\r\n"
                             "ZW1lbnQxOjA4BgNVBAMMMUFtYXpvbkRIQSBUdXlhX1Byb2R1Y3RfQTNHN0w4Wk9Q\r\n"
                             "Q01MU1ogQ2VydGlmaWNhdGUwWTATBgcqhkjOPQIBBggqhkjOPQMBBwNCAAT6x5p2\r\n"
                             "gMF55IxHIhYeZYvi9JHoIgf0f5DZFhLRew/4cyvp35/gg4qmS5TyJj/w0/kRlWlQ\r\n"
                             "92xqUSXCmV5Z7mz2o4IBFTCCAREwDwYDVR0TAQH/BAUwAwEB/zAdBgNVHQ4EFgQU\r\n"
                             "mJwEkREfWsOqylJ5UT53kvlC7kQwgc4GA1UdIwSBxjCBw4AU8RzI5oQ+OsSvsUub\r\n"
                             "ORRuOEETp/ShgZqkgZcwgZQxFDASBgNVBAoMC0FtYXpvbiBJbmMuMSMwIQYDVQQL\r\n"
                             "DBpEZXZpY2UgTWFuYWdlbWVudCBTZXJ2aWNlczEQMA4GA1UEBwwHU2VhdHRsZTET\r\n"
                             "MBEGA1UECAwKV2FzaGluZ3RvbjELMAkGA1UEBhMCVVMxIzAhBgNVBAMMGkFtYXpv\r\n"
                             "bkRIQSBSb290IENlcnRpZmljYXRlgg4AxujSrs5VP8cNqAB3ZjAOBgNVHQ8BAf8E\r\n"
                             "BAMCAYYwCgYIKoZIzj0EAwIDRwAwRAIgIUDhICIQzclWoZXHGFMIkFajYIO0DcYk\r\n"
                             "lFJVx5d+uHACIGy3fKfqenETqIH5snJsFR7QnEQ47Qq3K9ceGh2Azz1m\r\n"
                             "-----END CERTIFICATE-----\r\n"
                             "-----BEGIN CERTIFICATE-----\r\n"
                             "MIIDQDCCAuagAwIBAgIOAMbo0q7OVT/HDagAd2YwCgYIKoZIzj0EAwIwgZQxFDAS\r\n"
                             "BgNVBAoMC0FtYXpvbiBJbmMuMSMwIQYDVQQLDBpEZXZpY2UgTWFuYWdlbWVudCBT\r\n"
                             "ZXJ2aWNlczEQMA4GA1UEBwwHU2VhdHRsZTETMBEGA1UECAwKV2FzaGluZ3RvbjEL\r\n"
                             "MAkGA1UEBhMCVVMxIzAhBgNVBAMMGkFtYXpvbkRIQSBSb290IENlcnRpZmljYXRl\r\n"
                             "MB4XDTE5MTIwOTIwMjU0MFoXDTM5MTIwOTIwMjU0MFowgZwxCzAJBgNVBAYTAlVT\r\n"
                             "MRMwEQYDVQQIDApXYXNoaW5ndG9uMRAwDgYDVQQHDAdTZWF0dGxlMRgwFgYDVQQK\r\n"
                             "DA9BbWF6b24uY29tIEluYy4xGjAYBgNVBAsMEURldmljZSBNYW5hZ2VtZW50MTAw\r\n"
                             "LgYDVQQDDCdBbWF6b25ESEEgVHV5YV9NYW51ZmFjdHVyZXIgQ2VydGlmaWNhdGUw\r\n"
                             "WTATBgcqhkjOPQIBBggqhkjOPQMBBwNCAARBmDa+6dYNG2/KBpLv08bn6e1lmoKf\r\n"
                             "1hv0e8W3tQW97u1kFpiq3BF1MaQqrAiV0vqFItWOeWCXwV7oy3hz+6Q7o4IBEDCC\r\n"
                             "AQwwDwYDVR0TAQH/BAUwAwEB/zAdBgNVHQ4EFgQU8RzI5oQ+OsSvsUubORRuOEET\r\n"
                             "p/QwgckGA1UdIwSBwTCBvoAUYYvcTyRKpW7uCmW8vHSaUZXqq2ChgZqkgZcwgZQx\r\n"
                             "FDASBgNVBAoMC0FtYXpvbiBJbmMuMSMwIQYDVQQLDBpEZXZpY2UgTWFuYWdlbWVu\r\n"
                             "dCBTZXJ2aWNlczEQMA4GA1UEBwwHU2VhdHRsZTETMBEGA1UECAwKV2FzaGluZ3Rv\r\n"
                             "bjELMAkGA1UEBhMCVVMxIzAhBgNVBAMMGkFtYXpvbkRIQSBSb290IENlcnRpZmlj\r\n"
                             "YXRlggkAqkHFg3IfxuIwDgYDVR0PAQH/BAQDAgGGMAoGCCqGSM49BAMCA0gAMEUC\r\n"
                             "IQDnRZkmmDemoO+Mk7yutOIpp0mF2Cj4+1Y35uHLnobBfQIgOuZdBZK4Pab8TD2g\r\n"
                             "9TzvlsmQkQqm1VTNyqwykH0o+5g=\r\n"
                             "-----END CERTIFICATE-----\r\n"
                             "-----BEGIN CERTIFICATE-----\r\n"
                             "MIIDNjCCAtugAwIBAgIJAKpBxYNyH8biMAoGCCqGSM49BAMCMIGUMRQwEgYDVQQK\r\n"
                             "DAtBbWF6b24gSW5jLjEjMCEGA1UECwwaRGV2aWNlIE1hbmFnZW1lbnQgU2Vydmlj\r\n"
                             "ZXMxEDAOBgNVBAcMB1NlYXR0bGUxEzARBgNVBAgMCldhc2hpbmd0b24xCzAJBgNV\r\n"
                             "BAYTAlVTMSMwIQYDVQQDDBpBbWF6b25ESEEgUm9vdCBDZXJ0aWZpY2F0ZTAgFw0x\r\n"
                             "NzExMTYwNjI3MDlaGA8yMTE3MTEwODA2MjcwOVowgZQxFDASBgNVBAoMC0FtYXpv\r\n"
                             "biBJbmMuMSMwIQYDVQQLDBpEZXZpY2UgTWFuYWdlbWVudCBTZXJ2aWNlczEQMA4G\r\n"
                             "A1UEBwwHU2VhdHRsZTETMBEGA1UECAwKV2FzaGluZ3RvbjELMAkGA1UEBhMCVVMx\r\n"
                             "IzAhBgNVBAMMGkFtYXpvbkRIQSBSb290IENlcnRpZmljYXRlMFkwEwYHKoZIzj0C\r\n"
                             "AQYIKoZIzj0DAQcDQgAEv7+Zfvkc+qvUaKgaGxQMZoDFHQ18Z5OSXB4BNkYRszgR\r\n"
                             "PQ82o54KVb0RKkkq0e3niRn1gUZ8jozePEPrpPV5yaOCARAwggEMMA8GA1UdEwEB\r\n"
                             "/wQFMAMBAf8wHQYDVR0OBBYEFGGL3E8kSqVu7gplvLx0mlGV6qtgMIHJBgNVHSME\r\n"
                             "gcEwgb6AFGGL3E8kSqVu7gplvLx0mlGV6qtgoYGapIGXMIGUMRQwEgYDVQQKDAtB\r\n"
                             "bWF6b24gSW5jLjEjMCEGA1UECwwaRGV2aWNlIE1hbmFnZW1lbnQgU2VydmljZXMx\r\n"
                             "EDAOBgNVBAcMB1NlYXR0bGUxEzARBgNVBAgMCldhc2hpbmd0b24xCzAJBgNVBAYT\r\n"
                             "AlVTMSMwIQYDVQQDDBpBbWF6b25ESEEgUm9vdCBDZXJ0aWZpY2F0ZYIJAKpBxYNy\r\n"
                             "H8biMA4GA1UdDwEB/wQEAwIBhjAKBggqhkjOPQQDAgNJADBGAiEAoCM4t1cMuTeu\r\n"
                             "8yIlw/1BUIUb1Q4MYXp+LyfjcbmVz8ECIQDuKW8gQZjUS8Z7GcgNYnFup3UjTznj\r\n"
                             "5ja4/PmvDx0Glw==\r\n"
                             "-----END CERTIFICATE-----";

#endif

#endif

#endif
