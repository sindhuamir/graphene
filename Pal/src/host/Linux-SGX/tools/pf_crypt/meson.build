executable('graphene-sgx-pf-crypt',
    'pf_crypt.c',

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
