#include <iostream>
#include <fstream>
#include <cstdio>
#include <memory>
#include <stdexcept>
#include <string>

// Função para rodar o Nmap com um comando específico em um IP
std::string runNmap(const std::string& ipAddress, const std::string& nmapCommand) {
    std::string command = "nmap " + nmapCommand + " " + ipAddress;
    std::string result;

    // Abre um pipe para o comando
    std::shared_ptr<FILE> pipe(popen(command.c_str(), "r"), pclose);
    if (!pipe) throw std::runtime_error("Erro ao abrir o pipe!");

    // Lê a saída do comando e armazena em `result`
    char buffer[128];
    while (fgets(buffer, sizeof(buffer), pipe.get()) != nullptr) {
        result += buffer;
    }

    return result;
}

int main(int argc, char* argv[]) {
    // Verifica se o usuário forneceu o arquivo de alvos e o tipo de scan
    if (argc != 3) {
        std::cerr << "Uso: " << argv[0] << " <arquivo_alvos.txt> <tipo_scan>\n";
        std::cerr << "Tipos de scan:\n"
                  << "1 - Scan básico\n"
                  << "2 - Detecção de versão\n"
                  << "3 - Detecção de sistema operacional\n"
                  << "4 - Scan agressivo\n";
        return 1;
    }

    // Abre o arquivo de alvos
    std::ifstream alvoFile(argv[1]);
    if (!alvoFile) {
        std::cerr << "Erro ao abrir o arquivo de alvos: " << argv[1] << std::endl;
        return 1;
    }

    // Define o tipo de comando Nmap com base no argumento
    std::string nmapCommand;
    int scanType = std::stoi(argv[2]);  // Converte o argumento para um inteiro
    switch (scanType) {
        case 1:
            nmapCommand = "-sP";  // Scan básico (ping scan)
            break;
        case 2:
            nmapCommand = "-sV";  // Detecção de versão de serviços
            break;
        case 3:
            nmapCommand = "-O";   // Detecção de sistema operacional
            break;
        case 4:
            nmapCommand = "-A";   // Scan agressivo
            break;
        default:
            std::cerr << "Tipo de scan inválido! Escolha entre 1, 2, 3 ou 4.\n";
            return 1;
    }

    // Lê e escaneia cada alvo no arquivo
    std::string ipAddress;
    while (std::getline(alvoFile, ipAddress)) {
        if (ipAddress.empty()) continue;  // Pula linhas vazias

        std::cout << "Escaneando " << ipAddress << " com o comando: " << nmapCommand << "...\n";
        try {
            std::string scanResult = runNmap(ipAddress, nmapCommand);
            std::cout << "Resultado do escaneamento Nmap para " << ipAddress << ":\n"
                      << scanResult << "\n" << std::endl;
        } catch (const std::exception& e) {
            std::cerr << "Erro ao escanear " << ipAddress << ": " << e.what() << std::endl;
        }
    }

    return 0;
}