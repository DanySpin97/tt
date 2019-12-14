module libtt.section_parser;

import std.experimental.logger;

import confini;

import libtt.service;
import libtt.oneshot;

import libtt.service_parser;

int main_section_parser(string key, string value, ref Service service) {
    switch (key) {
        case "name":
            service.name = value;
            break;
        case "description":
            service.description = value;
            break;
        case "type":
            convert_service(service, value);
            break;
        default:
            criticalf("Key %s is not allowed in section [main]", key);
            return 1;
    }
    return 0;
}

int start_section_parser(string key, string value, ref Service service) {
    //auto oneshot = cast(Oneshot*)service;
    return 0;
}

int stop_section_parser(string key, string value, ref Service service) {
    return 0;
}

int run_section_parser(string key, string value, ref Service service) {
    return 0;
}

int environment_section_parser(string key, string value, ref Service service) {
    return 0;
}

int logger_section_parser(string key, string value, ref Service service) {
    return 0;
}
