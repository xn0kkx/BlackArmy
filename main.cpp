#include <iostream>
#include <fstream>
#include <cstdio>
#include <memory>
#include <stdexcept>
#include <string>

// Função para rodar o Nmap em um IP específico
std::string runNmap(const std::string& ipAddress) {
    std::string command = "nmap " + ipAddress;
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
    // Verifica se o usuário forneceu o arquivo de alvos como argumento
    if (argc != 2) {
        std::cerr << "Uso: " << argv[0] << " <arquivo_alvos.txt>" << std::endl;
        return 1;
    }

    // Abre o arquivo de alvos
    std::ifstream alvoFile(argv[1]);
    if (!alvoFile) {
        std::cerr << "Erro ao abrir o arquivo de alvos: " << argv[1] << std::endl;
        return 1;
    }

    std::string ipAddress;
    while (std::getline(alvoFile, ipAddress)) {  // Lê cada linha do arquivo
        if (ipAddress.empty()) continue; // Pula linhas vazias

        std::cout << "Escaneando " << ipAddress << "...\n";
        try {
            std::string scanResult = runNmap(ipAddress);
            std::cout << "Resultado do escaneamento Nmap para " << ipAddress << ":\n" 
                      << scanResult << "\n" << std::endl;
        } catch (const std::exception& e) {
            std::cerr << "Erro ao escanear " << ipAddress << ": " << e.what() << std::endl;
        }
    }

    return 0;
}
