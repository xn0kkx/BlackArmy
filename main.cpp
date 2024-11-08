#include <iostream>
#include <string>
#include <unordered_map>
#include <functional>  
#include <cstdlib>  

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

    std::cout <<
        "                      :::!~!!!!!:.                         \n"
        "                  .xUHWH!! !!?M88WHX:.                    \n"
        "                .X*#M@$!!  !X!M$$$$$$WWx:.               \n"
        "               :!!!!!!?H! :!$!$$$$$$$$$$8X:              \n"
        "              !!~  ~:~!! :~!$!#$$$$$$$$$$8X:             \n"
        "             :!~::!H!<   ~.U$X!?R$$$$$$$$MM!             \n"
        "             ~!~!!!!~~ .:XW$$$U!!?$$$$$$RMM!             \n"
        "               !:~~~ .:!M\"T#$$$$WX??#MRRMMM!            \n"
        "               ~?WuxiW*`   `\"#$$$$8!!!!???!!!            \n"
        "             :X- M$$$$       `\"T#$T~!8$WUXU~           \n"
        "            :%`  ~#$$$m:        ~!~ ?$$$$$$            \n"
        "          :!`.-   ~T$$$$8xx.  .xWW- ~\"\"##*\"          \n"
        ".....   -~~:<` !    ~?T#$$@@W@*?$$      /`           \n"
        "W$@@M!!! .!~~ !!     .:XUW$W!~ `\"~:    :            \n"
        "#\"~~`.:x%`!!  !H:   !WM$$$$Ti.: .!WUn+!            \n"
        ":::~:!!`:X~ .: ?H.!u \"$$$B$$$!W:U!T$$M~          \n"
        ".~~   :X@!.-~   ?@WTWo(\"*$$$W$TH$!               \n"
        "Wi.~!X$?!-~    : ?$$$B$Wu(\"**$RM!                \n"
        "$R@i.~~ !     :   ~$$$$$B$$en:``                \n"
        "?MXT@Wx.~    :     ~\"##*$$$$M~               \n\n";  
}
void show_help(){
    std::cout << "Black Army command list:\n";
    std::cout << "help - Display the command list\n";
    std::cout << "exit - Close the BlackArmy\n";
    std::cout << "clear - Clear the screen\n";
    std::cout << "help - Display the command list\n";
    std::cout << "help - Display the command list\n";
    std::cout << "help - Display the command list\n";
}

void clear_screen(){
    std::system("clear");
}

int main() {
    std::unordered_map<std::string, std::function<void()>> commands;

    commands["help"] = show_help;
    commands["clear"] =clear_screen;

    std::string command;
    

    print_ascii_art();
    
    while (true) {
        show_prompt();
        if (!std::getline(std::cin, command)){
            std::cout << std::endl;
            std::cout << "See ya!";
            break;
        } 
        
        if (command == "exit") {
            std::cout << "See ya!\n";
            break;
        } else if (commands.find(command) != commands.end()){
            commands[command]();
        } else if (!command.empty()) {
            std::system(command.c_str());
        }
    }

    return 0;
}