executable('graphene-sgx-quote-dump',
    'quote_dump.c',

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
