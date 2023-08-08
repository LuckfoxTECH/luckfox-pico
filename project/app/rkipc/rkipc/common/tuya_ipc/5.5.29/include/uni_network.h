/**
 * @file uni_network.h
 *
 * @brief Tuya iotdns module is the DNS service module provided by Tuya, which ensures that
 * developers can provide reliable, safe and stable DNS services when accessing the network.
 * Tuya iotdns provides a DNS mechanism of mutual protection for static zone DNS resolution,
 * dynamic random zone DNS resolution and local system DNS resolution. Among them, the DNS
 * resolution mechanism relies on Tuya The trusted DNS service provided in the cloud can obtain
 * the optimal DNS policy according to the zone and provide a load balancing mechanism; the dynamic
 * random zone DNS resolution is based on the zone DNS resolution mechanism. In order to avoid a
 * protection mechanism provided by DNS failure 2, a random zone is selected in all non current
 * zones for DNS request; the local system DNS is provided by the local operator As a backing
 * mechanism, the basic DNS service of. Tuya iotdns can switch the resolution mechanism in case of
 * failure to ensure the stability and security of the whole mechanism.
 *
 * @version 1.0
 *
 * @date 2019-09-09
 *
 * @copyright Copyright (c) tuya.inc 2019
 *
 */

#ifndef __TUYA_BASE_UNI_DNS_CACHE_H__
#define __TUYA_BASE_UNI_DNS_CACHE_H__

#include "tuya_cloud_types.h"
#include "tuya_hal_network.h"
#include "tuya_iot_config.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief max length of domain name
 *
 */
#define MAX_DOMAIN_NAME_LEN 128

/**
 * @brief each TLS MODE have specific iot-dns domain name
 *
 */
#if defined(TLS_MODE) && (TLS_MODE != TLS_DISABLE)
#if defined(TLS_MODE) && ((TLS_MODE == TLS_TUYA_PSK_ONLY) || (TLS_MODE == TLS_TUYA_ECC_PSK))
// TLS_MODE==TLS_TUYA_PSK_ONLY or TLS_MODE==TLS_TUYA_ECC_PSK
#define HTTP_DNS_SERVER_DOMAIN "h3.iot-dns.com"
#define HTTP_DNS_SERVER_DOMAIN_WE "h3-we.iot-dns.com"
#elif defined(TLS_MODE) && (TLS_MODE == TLS_TUYA_ECC_CLIENT_AUTH)
// TLS_MODE==TLS_TUYA_ECC_CLIENT_AUTH
#define HTTP_DNS_SERVER_DOMAIN "h4.iot-dns.com"
#define HTTP_DNS_SERVER_DOMAIN_WE "h4-we.iot-dns.com"
#else
// TLS_MODE==TLS_TUYA_PSK_ONLY or TLS_MODE==TLS_TUYA_ECC_ONLY_NOSTRIP
#define HTTP_DNS_SERVER_DOMAIN "h2.iot-dns.com"
#define HTTP_DNS_SERVER_DOMAIN_WE "h2-we.iot-dns.com"
#endif
#else
// TLS_MODE==DISABLE
#error "tls mode must be set."
#endif

/**
 * @brief DNS region
 *
 */
typedef UINT_T REGION_E;
#define E_REGION_DYNAMIC 0 // no fixed region
#define E_REGION_CN 1      // china region
#define E_REGION_EU 2      // european region
#define E_REGION_US 3      // american region
#define E_REGION_UE 4      // west american region
#define E_REGION_IN 5      // indian region
#define E_REGION_WE 6      // west european region
#define E_REGION_MAX 7     // region max

/**
 * @brief DNS priority
 *
 */
typedef UINT_T TY_DNS_PRIO_T;
#define DNS_PRIO_REGION 0 // default priority, use the dns server of current region
#define DNS_PRIO_RANDOM 1 // select a random dns server of the regions except current region
#define DNS_PRIO_SYSTEM 2 // use the system dns service of the router

/**
 * @brief DNS entry info
 *
 */
typedef struct {
	REGION_E region;  // the region of the DNS entry
	CHAR_T *domain;   // the domain of the DNS entry
	UNW_IP_ADDR_T ip; // the ip address of the DNS entry
} TY_DNS_INFO_S;

/**
 * @brief get the domain ip address info by the domain name
 *
 * @param[in] domain the domain name
 * @param[out] addr the ip address of the domain
 * @return OPRT_OK: success, others: fail, please refer to tuya_error_code.h
 */
OPERATE_RET unw_gethostbyname(CONST CHAR_T *domain, UNW_IP_ADDR_T *addr);

/**
 * @brief connect to a domain with specific tcp port
 *
 * @param[out] sockfd the socket fd
 * @param[in] hostname the domian name
 * @param[in] port the tcp port
 * @param[in] retry_cnt retry count if tcp connect failed
 * @param[in] timeout timeout in ms
 * @return OPRT_OK: success, others: fail, please refer to tuya_error_code.h
 */
OPERATE_RET unw_connect_to_domain(INT_T *sockfd, CHAR_T *hostname, UINT16_T port, UINT_T retry_cnt,
                                  UINT_T timeout);

/**
 * @brief init the dns module
 *
 * @return OPRT_OK: success, others: fail, please refer to tuya_error_code.h
 */
OPERATE_RET unw_init_dns_cache(VOID);

/**
 * @brief clean all dynamic dns cache
 *
 * @return OPRT_OK: success, others: fail, please refer to tuya_error_code.h
 */
OPERATE_RET unw_clear_all_dns_cache(VOID);

/**
 * @brief clear the specific domain dns cache
 *
 * @param[in] domain the domian which need to clear
 * @return OPRT_OK: success, others: fail, please refer to tuya_error_code.h
 */
OPERATE_RET unw_clear_dns_cache(CONST CHAR_T *domain);

/**
 * @brief add dns information to dynamic dns cache
 *
 * @param[in] domain the domian name
 * @param[in] ip the ip address of the domain name
 * @return OPRT_OK: success, others: fail, please refer to tuya_error_code.h
 */
OPERATE_RET unw_add_dns_cache(CONST CHAR_T *domain, CONST UNW_IP_ADDR_T ip);

/**
 * @brief set the dns current region
 *
 * @param[in] region the new current region
 * @return OPRT_OK: success, others: fail, please refer to tuya_error_code.h
 */
OPERATE_RET unm_set_dns_region(REGION_E region);

/**
 * @brief set the dns priority
 *
 * @param[in] dns_prio the new dns priority
 * @return OPRT_OK: success, others: fail, please refer to tuya_error_code.h
 */
OPERATE_RET unm_set_dns_cache_priority(TY_DNS_PRIO_T dns_prio);

/**
 * @brief get the dns current priority
 *
 * @return REGION_E the current priority
 */
TY_DNS_PRIO_T unm_get_dns_cache_priority(VOID);

/**
 * @brief decrease the dns current priority
 *
 * @note if the priority is DNS_PRIO_SYSTEM, lower dns priority will return to DNS_PRIO_REGION
 */
VOID unm_lower_dns_cache_priority(VOID);

#ifdef __cplusplus
}
#endif

#endif // __TUYA_BASE_UNI_DNS_CACHE_H__
