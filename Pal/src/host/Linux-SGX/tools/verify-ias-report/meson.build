executable('graphene-sgx-ias-verify-report',
    'verify_ias_report.c',

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
