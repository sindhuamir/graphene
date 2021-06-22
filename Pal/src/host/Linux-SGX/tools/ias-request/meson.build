executable('graphene-sgx-ias-request',
    'ias_request.c',

    args: [
        '-D_GNU_SOURCE',
    ],

    dependencies: [
        common_dep,
        sgx_util_dep,
        mbedtls_files_dep,
    ],

    install: true,
)
