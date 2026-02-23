#include "utils.h"

constexpr unsigned int hash(const char* str) {
    unsigned int h = 0;
    while (*str) {
        h = h * 31 + *str++; 
    }
    return h;
}

ipAddress* parseIpAddress(const char* str) {
    unsigned long bytes[4];
    char*end = nullptr;
    size_t counter = 0;

    for (long i = std::strtol(str, &end,10);
        str!=end;
        i=std::strtol(str,&end,10)) {

        if (constexpr long MAX_BYTE = 255; i < 0 || i > MAX_BYTE) break;
        bytes[counter] = i;

        str=end; if (*str=='.') ++str;
        counter++;

        if (counter==4) break;

        }
    if (counter!=4 || *end != '\0') {
        std::cout<<"Ошибка во время парсинга айпи адреса\n";
        return nullptr;
    }
    return new ipAddress(bytes[0],
        bytes[1],
        bytes[2],
        bytes[3]);

}


std::ostream& operator<<(std::ostream& stream, const ipAddress& addr) {
    stream
    << static_cast<unsigned short>(addr.b1) <<'.'
    << static_cast<unsigned short>(addr.b2) << '.'
    << static_cast<unsigned short>(addr.b3) << '.'
    << static_cast<unsigned short>(addr.b4);

    return stream;
}

long parsePort(const char* str) {
    char*end = nullptr;
    const long tempPort = strtol(str, &end, 10);
    if (*end != '\0') {
        return -1;
    }
    if (constexpr unsigned long MAX_PORT = 65535;
        tempPort > MAX_PORT) {
        std::cout << "Порт должен быть не больше 65535\n";
        return -1;
        }
    return tempPort;
}

App* parseCommandArgs(const int argc, char*argv[]) {
    const auto app = new App;
    unsigned long tempPort = 0;

    if (argc%2==0 || argc<2){
        std::cout << "Неверное число аргументов командной строки\n";
        delete app;
        return nullptr;
    }
    for (int i = 1; i < argc; i+=2){

        const char* flag = argv[i];
        const char* value = argv[i + 1];
        ipAddress* addr = nullptr;
        char*end = nullptr;

        switch (hash(flag)){
            case hash("-p"):
                tempPort = parsePort(value);
                if (tempPort==-1) {
                    delete app;
                    return nullptr;
                }
                app->port = tempPort;
                break;

            case hash("-a"):
                addr = parseIpAddress(value);
                if (addr) {
                    app->addr = addr;
                    break;
                }
                delete app;
                return nullptr;

            case hash("-r"):
                switch (hash(value)){
                    case hash("Client"):
                        app->role = Role::CLIENT;
                        break;
                    case hash("Admin"):
                        app->role = Role::ADMIN;
                        break;
                    default:
                        std::cout << "Неизвестная роль: " << value << std::endl;
                        delete app;
                        return nullptr;
                }
                break;

            case hash("-L"): 
                app->lib = value;
                break;
            case hash("-i"):
                app->i = std::strtol(value, &end, 10);
                break;
            default:
                std::cout<<"Неверный флаг\n";
                delete app;
                return nullptr;
        }
    }
    return app;
}

