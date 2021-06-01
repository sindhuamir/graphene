sgx_util_so = shared_library('sgx_util',
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
        '-D_POSIX_C_SOURCE=200809L',
    ],

    include_directories: include_directories(
        '../..',
        '../../../../../../common/include',
        '../../../../../../common/include/arch/$(ARCH)',
        '../../../../../../common/src/crypto/mbedtls/install/include',
    ),

    link_args: [
    ],

    dependencies: [
        cjson_dep,
        libcurl_dep,
        protected_files_dep,
    ],
)
