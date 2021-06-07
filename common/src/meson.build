common_dep = declare_dependency(
    'avl_tree.c',
    'path.c',
    'stack_protector.c',

    'network/hton.c',
    'network/inet_pton.c',

    'string/atoi.c',
    'string/ctype.c',
    'string/memcmp.c',
    'string/memcpy.c',
    'string/memset.c',
    'string/strchr.c',
    'string/strcmp.c',
    'string/strlen.c',
    'string/strspn.c',
    'string/strstr.c',
    'string/toml_utils.c',
    'string/utils.c',

    include_directories: include_directories(
        '../include',
        join_paths('../include/arch', host_machine.cpu_family()),
    ),

    dependencies: [
        uthash_dep,
        toml_dep,
        mbedtls_dep,
    ],
)
