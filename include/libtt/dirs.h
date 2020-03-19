// Copyright 2020 Danilo Spinella <danyspin97@protonmail.com>
// Distributed under the terms of the GNU General Public License v2

#ifdef __cplusplus
namespace tt {
#endif // __cplusplus

static constexpr const char* = "@version@";

struct Dirs
{
    static constexpr char* bin = "@bindir@";
    static constexpr char* conf = "@confdir@";
    static constexpr char* execlinePrefix = "@execline_prefix@";
    static constexpr char* libexec = "@libexecdir@";
    static constexpr char* live = "@livedir@";
    static constexpr char* log = "@logdir@";
    static constexpr char* service = "@servicedir@";
    static constexpr char* system = "@systemdir@";

    static constexpr char* customScripts = "@confdir@";
    static constexpr char* adminService = "@confdir@/service";
    static constexpr char* adminConf = "@confdir@/conf";
} dirs;

#ifdef __cplusplus
}
#endif // __cplusplus
