#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdbool.h>
#include <pthread.h>

struct ThreadData {
    bool* continue_progress;
};

void* showProgress(void* arg) {
    const char* progressChars = "|/-\\";
    int idx = 0;
    struct ThreadData* data = (struct ThreadData*)arg;
    while (*(data->continue_progress)) {
        printf("\rAguarde... %c", progressChars[idx]);
        fflush(stdout);
        idx = (idx + 1) % 4;
        sleep(1);
    }
    return NULL;
}

void show_manual() {
    printf("Este programa permite a instalação de pacotes .deb em distruições RPM.\n\n");
    printf("Uso:\n");
    printf("sudo %s arquivo.deb\n\n", __FILE__);
    printf("Exemplo:\n");
    printf("sudo %s meu_pacote.deb\n\n", __FILE__);
    printf("Opções:\n");
    printf("--h, -help       Mostra este manual de ajuda\n");
}

int main(int argc, char *argv[]) {
    if (argc == 2 && (strcmp(argv[1], "--h") == 0 || strcmp(argv[1], "-help") == 0)) {
        show_manual();
        return 0;
    }

    if (argc != 2) {
        printf("Número incorreto de argumentos.\n");
        show_manual();
        return 1;
    }

    if (geteuid() != 0) {
        printf("Este script precisa ser executado com sudo.\n");
        return 1;
    }

    printf("Bem-vindo ao Portador de .deb para distribuições RPM\n");
    printf("==================================================\n\n");

    if (access(argv[1], F_OK) == -1) {
        printf("Arquivo %s não encontrado.\n", argv[1]);
        return 1;
    }

    char temp_dir[] = "/tmp/dpm_temp.XXXXXX";
    if (mkdtemp(temp_dir) == NULL) {
        printf("Erro ao criar diretório temporário.\n");
        return 1;
    }

    bool continue_progress = true;
    struct ThreadData data = { .continue_progress = &continue_progress };
    pthread_t progress_thread;
    pthread_create(&progress_thread, NULL, showProgress, (void*)&data);

    char dpkg_command[512];
    snprintf(dpkg_command, sizeof(dpkg_command), "sudo dpkg-deb -R %s %s >/dev/null 2>&1", argv[1], temp_dir);
    if (system(dpkg_command) != 0) {
        printf("Erro ao extrair o arquivo .deb.\n");
        return 1;
    }

    char copy_command[512];
    snprintf(copy_command, sizeof(copy_command), "sudo cp -r %s/* /", temp_dir);
    if (system(copy_command) != 0) {
        printf("Erro ao copiar os arquivos extraídos.\n");
        return 1;
    }

    char postinst_path[256];
    snprintf(postinst_path, sizeof(postinst_path), "%s/DEBIAN/postinst", temp_dir);
    if (access(postinst_path, F_OK) == 0) {

        FILE *postinst_file = fopen(postinst_path, "r");
        if (postinst_file == NULL) {
            printf("Erro ao abrir o arquivo postinst.\n");
            return 1;
        }

        FILE *desktop_creation_file = fopen("/tmp/desktop_creation.sh", "w");
        if (desktop_creation_file == NULL) {
            printf("Erro ao criar o arquivo desktop_creation.sh.\n");
            return 1;
        }

        FILE *alias_creation_file = fopen("/tmp/alias_creation.sh", "w");
        if (alias_creation_file == NULL) {
            printf("Erro ao criar o arquivo alias_creation.sh.\n");
            return 1;
        }

        char line[512];
        int desktop_creation = 0;
        int alias_creation = 0;
        while (fgets(line, sizeof(line), postinst_file) != NULL) {
            if (strcmp(line, "# Create desktop file\n") == 0) {
                desktop_creation = 1;
            } else if (strcmp(line, "# End of desktop file creation\n") == 0) {
                desktop_creation = 0;
            } else if (strcmp(line, "# Create alias\n") == 0) {
                alias_creation = 1;
            } else if (strcmp(line, "# End of alias creation\n") == 0) {
                alias_creation = 0;
            }

            if (desktop_creation) {
                fputs(line, desktop_creation_file);
            } else if (alias_creation) {
                fputs(line, alias_creation_file);
            }
        }

        fclose(postinst_file);
        fclose(desktop_creation_file);
        fclose(alias_creation_file);

        if (system("sudo chmod +x /tmp/desktop_creation.sh") != 0 || system("sudo /tmp/desktop_creation.sh") != 0) {
            printf("Erro ao executar o script desktop_creation.sh.\n");
            return 1;
        }

        if (system("sudo chmod +x /tmp/alias_creation.sh") != 0 || system("sudo /tmp/alias_creation.sh") != 0) {
            printf("Erro ao executar o script alias_creation.sh.\n");
            return 1;
        }
    }
	
    char find_command[512];
    snprintf(find_command, sizeof(find_command), "find %s -type f -name '*.desktop'", temp_dir);
    FILE *desktop_file = popen(find_command, "r");
    char desktop_path[256];

    while (fgets(desktop_path, sizeof(desktop_path), desktop_file) != NULL) {
        char copy_desktop_command[512];
        size_t len = strlen(desktop_path);
        if (len > 0 && desktop_path[len - 1] == '\n') {
            desktop_path[len - 1] = '\0';
        }
        snprintf(copy_desktop_command, sizeof(copy_desktop_command), "sudo cp %s /usr/share/applications/", desktop_path);
        if (system(copy_desktop_command) != 0) {
            printf("Erro ao copiar o arquivo .desktop.\n");
            return 1;
        }


    }

pclose(desktop_file); // close the file stream

    printf("\nLimpando...\n");
    char cleanup_command[512];
    snprintf(cleanup_command, sizeof(cleanup_command), "sudo rm -rf %s", temp_dir);
    if (system(cleanup_command) != 0) {
        printf("Erro ao limpar o diretório temporário.\n");
        return 1;
    }

    printf("Concluído.\n");
    continue_progress = false;

    pthread_join(progress_thread, NULL);

    return 0;
}
