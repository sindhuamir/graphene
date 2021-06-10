mbedtls_common_dep = declare_dependency(
    dependencies: mbedtls_files_dep,
    compile_args: [
        '-D',
    ],
)

sgx_util = shared_library('sgx_util',
    'attestation.c',
    'attestation.h',
    'ias.c',
    'ias.h',
    'pf_util.c',
    'pf_util.h',
    'util.c',
    'util.h',

    args: [
        '-DCRYPTO_USE_MBEDTLS',

        # TODO this should probably be written into the files
        '-D_POSIX_C_SOURCE=200809L',
    ],

    include_directories: include_directories(
        '../../../../../../common/src/crypto/mbedtls/install/include',
    ),

    link_args: [
    ],

    dependencies: [
        common_dep,
        cjson_dep,
        libcurl_dep,
        protected_files_dep,
        mbedtls_dep,
    ],
)

sgx_util_dep = declare_dependency(
    link_with: sgx_util,
)
