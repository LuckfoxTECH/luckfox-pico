project (xtest-ta-headers C)

add_library(${PROJECT_NAME} INTERFACE)

target_include_directories(${PROJECT_NAME}
	INTERFACE include
	INTERFACE aes_perf/include
	INTERFACE concurrent/include
	INTERFACE concurrent_large/include
	INTERFACE create_fail_test/include
	INTERFACE crypt/include
	INTERFACE enc_fs/include
	INTERFACE os_test/include
	INTERFACE rpc_test/include
	INTERFACE sdp_basic/include
	INTERFACE sha_perf/include
	INTERFACE sims/include
	INTERFACE miss/include
	INTERFACE sims_keepalive/include
	INTERFACE socket/include
	INTERFACE storage_benchmark/include
	INTERFACE tpm_log_test/include
	INTERFACE supp_plugin/include
)
