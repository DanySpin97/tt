module libtt.env;

import std.conv : to ;
import std.stdio : writeln ;
import std.string : fromStringz, toStringz ;

import std.experimental.logger;

import confini;

import libtt.define;
import libtt.dirs;

struct EnvWrapper {
    string[string] hash;
}

extern (C) IniCallback get_env_parse_callback() {
    return (IniDispatch* dispatch, void* env_p) {
        ini_unquote(dispatch.data, dispatch.format);
        ini_string_parse(dispatch.value, dispatch.format);
        auto env = cast(EnvWrapper*)env_p;
        auto key = to!string(dispatch.data);
        auto val = to!string(dispatch.value);
        env.hash[key] = val;

        return 0;
    };
}

IniFormat get_env_format() {
    IniFormat env_format;

    env_format.delimiter_symbol = IniDelimiters.INI_EQUALS;
    env_format.case_sensitive = 1;
    env_format.semicolon_marker = IniCommentMarker.INI_IGNORE;
    env_format.hash_marker = IniCommentMarker.INI_IGNORE;
    env_format.multiline_nodes = IniMultiline.INI_NO_MULTILINE;
    env_format.section_paths = IniSectionPaths.INI_NO_SECTIONS;
    env_format.no_single_quotes = 1;
    env_format.no_double_quotes = 0;
    env_format.no_spaces_in_names = 1;
    env_format.implicit_is_not_empty = 1;

    return env_format;
}


EnvWrapper parse_env(string path) {
    EnvWrapper env;

    uint ret = load_ini_path(
                toStringz(path),
                get_env_format(),
                null,
                get_env_parse_callback(),
                &env);

    if (ret == CONFINI_ERROR) {
        criticalf("Something went wrong when parsing file %s", path);
    } else if(ret == ConfiniInterruptNo.CONFINI_ENOMEM) {
        criticalf("Could not alloc memory when reading file %s", path);
    } else if (ret == ConfiniInterruptNo.CONFINI_ENOENT) {
        logf("No file at: %s", path);
    } else if (ret == ConfiniInterruptNo.CONFINI_EIO) {
        criticalf("Could not read file at: %s", path);
    }

    logf("Loaded file with path: %s", path);

    return env;
}

