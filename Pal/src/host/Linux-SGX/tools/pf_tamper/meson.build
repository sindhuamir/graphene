executable('graphene-sgx-pf-tamper',
    'pf_tamper.c',

    args: [
        '-D_GNU_SOURCE',
    ],

    dependencies: [
        common_dep,
        sgx_util_dep,
        mbedtls_files_dep,
        protected_files_dep,
    ],

    install: true,
)
