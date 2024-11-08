#include <iostream>
#include <string>
#include <cstdlib>  // Para o sistema de execução de comandos

void show_prompt() {
    std::cout << "BlackArmy> ";
}

void print_ascii_art() {
    std::cout <<
        "______  _               _      ___                          \n"
        "| ___ \\| |             | |    / _ \\                         \n"
        "| |_/ /| |  __ _   ___ | | __/ /_\\ \\ _ __  _ __ ___   _   _ \n"
        "| ___ \\| | / _` | / __|| |/ /|  _  || '__|| '_ ` _ \\ | | | |\n"
        "| |_/ /| || (_| || (__ |   < | | | || |   | | | | | || |_| |\n"
        "\\____/ |_| \\__,_| \\___||_|\\_\\\\_| |_/|_|   |_| |_| |_| \\__, |\n"
        "                                                       __/ | \n"
        "                                                      |___/  \n";
}

int main() {
    std::string command;
    
    // Exibe a arte ASCII ao iniciar
    print_ascii_art();
    
    while (true) {
        show_prompt();
        std::getline(std::cin, command);  // Obtém o comando do usuário
        
        if (command == "exit") {
            std::cout << "Saindo do terminal...\n";
            break;
        }
        
        // Caso o comando não seja "exit", executa como se fosse um comando do shell
        if (!command.empty()) {
            std::system(command.c_str());
        }
    }

    return 0;
}
