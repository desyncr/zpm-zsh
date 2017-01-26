/* vim: set ts=4 sw=4 expandtab: */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <limits.h>

#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>

#define TRUE 1
#define FALSE 0

char* generate_plugin_path(char* plugin_name) {
    char* plugin_path = malloc(PATH_MAX);
    strcpy(plugin_path, getenv("HOME"));
    strcat(plugin_path, "/.zpm/plugins/");
    strcat(plugin_path, plugin_name);

    return plugin_path;
}

char* get_plugin_entry_point(char* plugin_name) {
    DIR *dir;
    struct dirent *ent;
    char* plugin_path = generate_plugin_path(plugin_name);
    char* plugin_entry_point = malloc(PATH_MAX);
    strcpy(plugin_entry_point, "");

    if ((dir = opendir(plugin_path)) != NULL) {
      while (strlen(plugin_entry_point) == 0
                && (ent = readdir (dir)) != NULL) {
        if (strstr(ent->d_name, ".plugin.zsh")) {
            // x.plugin.zsh entry point
            strcpy(plugin_entry_point, ent->d_name);
        } else if (strstr(ent->d_name, "init.zsh")) {
            // prezto like
            strcpy(plugin_entry_point, ent->d_name);
        } else if (strstr(ent->d_name, ".zsh")) {
            strcpy(plugin_entry_point, ent->d_name);
        } else if (strstr(ent->d_name, ".sh")) {
            strcpy(plugin_entry_point, ent->d_name);
        }
      }

      closedir (dir);
    }

    free(plugin_path);
    return plugin_entry_point;
}

char* get_zpm_init_path() {
    char* zpm_init = malloc(PATH_MAX);

    strcpy(zpm_init, getenv("HOME"));
    strcat(zpm_init, "/.zpm-init.zsh");

    return zpm_init;
}

char* get_plugin_entry(char* plugin_name) {
    char* plugin_entry = malloc(PATH_MAX);

    char* plugin_path = generate_plugin_path(plugin_name);

    char* plugin_entry_point = get_plugin_entry_point(plugin_name);

    strcpy(plugin_entry, "source ");
    strcat(plugin_entry, plugin_path);
    strcat(plugin_entry, "/");
    strcat(plugin_entry, plugin_entry_point);
    strcat(plugin_entry, "\n");

    free(plugin_path);
    free(plugin_entry_point);
    return plugin_entry;
}

int generate_plugin_entry(char* plugin_name) {
    char* zpm_init = get_zpm_init_path();
    FILE* store = fopen(zpm_init,"ab+");

    char* plugin_entry = get_plugin_entry(plugin_name);
    char* plugin_entry_list = malloc(PATH_MAX);
    fread(plugin_entry_list, 1, 1024, store);
    fclose(store);
    if (strstr(plugin_entry_list, plugin_entry)) {
        free(zpm_init);
        free(plugin_entry);
        free(plugin_entry_list);
        return 0;
    }

    FILE *init = fopen(zpm_init, "r+");
    fseek(init, -54, SEEK_END);
    strcat(plugin_entry, "autoload -Uz compinit; compinit -iCd $HOME/.zcompdump\n");

    int status = fwrite(plugin_entry, strlen(plugin_entry), 1, init);
    fclose(init);

    free(zpm_init);
    free(plugin_entry);
    free(plugin_entry_list);
    return status;
}

char* get_plugin_list_path() {
    char* plugin_list = malloc(PATH_MAX);

    strcpy(plugin_list, getenv("HOME"));
    strcat(plugin_list, "/.zpm/.plugin_list");

    return plugin_list;
}

int plugin_list_add_item(char* plugin_name) {
    int ret;
    char* plugin_item = malloc(PATH_MAX);
    strcpy(plugin_item, plugin_name);
    strcat(plugin_item, "\n");

    char* plugin_list = get_plugin_list_path();
    FILE* store = fopen(plugin_list,"ab+");

    char* plugin_item_list = malloc(PATH_MAX);
    fread(plugin_item_list, 1, 1024, store);
    if (strstr(plugin_item_list, plugin_item)) {
        free(plugin_item);
        free(plugin_list);
        free(plugin_item_list);
        return 0;
    }

    ret = fwrite(plugin_item, strlen(plugin_item), 1, store);
    free(plugin_item);
    free(plugin_list);
    free(plugin_item_list);
    return ret;
}

/* https://gist.github.com/JonathonReinhart/8c0d90191c38af2dcadb102c4e202950 */
int mkdir_p(const char *path) {
    /* Adapted from http://stackoverflow.com/a/2336245/119527 */
    const size_t len = strlen(path);
    char _path[PATH_MAX];
    char *p;

    errno = 0;

    /* Copy string so its mutable */
    if (len > sizeof(_path)-1) {
        errno = ENAMETOOLONG;
        return -1; 
    }
    strcpy(_path, path);

    /* Iterate the string */
    for (p = _path + 1; *p; p++) {
        if (*p == '/') {
            /* Temporarily truncate */
            *p = '\0';

            if (mkdir(_path, S_IRWXU) != 0) {
                if (errno != EEXIST)
                    return -1; 
            }

            *p = '/';
        }
    }

    if (mkdir(_path, S_IRWXU) != 0) {
        if (errno != EEXIST)
            return -1; 
    }

    return 0;
}

int local_clone_exists(char* plugin_name) {
    char* plugin_path = generate_plugin_path(plugin_name);
    DIR* plugin_directory = opendir(plugin_path);

    if (plugin_directory != NULL) {
        closedir(plugin_directory);
        free(plugin_path);
        return 0;
    }
    /*int status = */mkdir_p(plugin_path);
    free(plugin_path);

    return 1;
}

int zpm_configuration_exists() {
    char* zpm_init_path = get_zpm_init_path();

    FILE *file;
    if ((file = fopen(zpm_init_path, "r")) == NULL) {
        return -1;
    } else {
        fclose(file);
    }

    return 0;
}

char* generate_repository_url(char* plugin_name) {
    char* url = malloc(PATH_MAX);
    char* tmp = strstr(plugin_name, "/");

    tmp = strstr(tmp + 1, "/");
    if (!tmp) {
        strcpy(url, "https://github.com/");
    } else {
        strcpy(url, "https://");
    }
    strcat(url, plugin_name);

    return url;
}

int locally_clone_plugin(char* plugin_name) {
    int ret;
    char* repository_url = generate_repository_url(plugin_name);
    char* clone_destination = generate_plugin_path(plugin_name);

    char* command = malloc(PATH_MAX);
    strcpy(command, "git clone --recursive --depth=1 ");
    strcat(command, repository_url);
    strcat(command, " ");
    strcat(command, clone_destination);
    strcat(command, " > /dev/null 2>&1");

    free(repository_url);
    free(clone_destination);

    ret = system(command);
    free(command);
    return ret;
}

char* get_zpm_plugin_list() {
    char* plugin_list_path = get_plugin_list_path();
    char* listing = malloc(1024);
    FILE* list;

    list = fopen(plugin_list_path, "rb");
    if (list != NULL) {
        fread(listing, 1, 1024, list);
        fclose(list);
    } else {
        strcpy(listing, "Nothing to show.");
    }

    free(plugin_list_path);

    return listing;
}

int plugins_update_local_clone() {
    int ret;
    char* command = malloc(PATH_MAX);
    char* listing = get_zpm_plugin_list();
    char* plugin_name = strtok(listing, "\n");

    if(!strcmp(listing, "Nothing to show.")) {
      printf("Nothing to update.");
      return -1;
    }
    printf("Updating plugins ...\n");
    while(plugin_name) {
      strcpy(command, "cd ~/.zpm/plugins/");
      strcat(command, plugin_name);
      strcat(command, "; git pull");
      printf("Updating %s...\n", plugin_name);
      ret = system(command);
      plugin_name = strtok(NULL, "\n");
    }
    free(command);
    free(listing);
    return ret;
}

void usage() {
    printf("%s\n", "Usage:\n\tzpm 'zsh-users/zsh-syntax-highlighting'");
    printf("%s\n", "\nAvailable commands:\n\tzpm reset\n\tzpm list");
    printf("%s\n", "\tzpm update\n\tzpm help");
}

int main(int argc, char* argv[]) {
    if (argc <= 1) {
        usage();
        return 1;
    }

    char* plugin_name_or_command = argv[1];
    char* plugin_name = NULL;

    if (strstr(plugin_name_or_command, "reset")) {
        char* plugin_list = get_plugin_list_path();
        char* zpm_init = get_zpm_init_path();
        unlink(plugin_list);
        unlink(zpm_init);
        free(plugin_list);
        free(zpm_init);
        return 0;

    } else if (strstr(plugin_name_or_command, "update")) {
        plugins_update_local_clone();
        return 0;

    } else if (strstr(plugin_name_or_command, "list")) {
        char* plugin_list = get_zpm_plugin_list();
        printf("%s\n", plugin_list);
        free(plugin_list);
        return 0;

    } else if (strstr(plugin_name_or_command, "help")) {
        usage();
        return 0;

    } else {
        plugin_name = malloc(PATH_MAX);
        strcpy(plugin_name, plugin_name_or_command);
    }

    int status = 0;
    char* install = malloc(1024);
    strcpy(install, "Installing ");
    strcat(install, plugin_name);
    strcat(install, "... ");
    printf("%s", install);
    fflush(stdout);

    if (local_clone_exists(plugin_name) == 1) {
        status = locally_clone_plugin(plugin_name);
    }

    if (status == 0) {
        printf("%s\n", "Done.");
        generate_plugin_entry(plugin_name);
        plugin_list_add_item(plugin_name);
    } else {
        printf("%s\n", "Error!");
    }

    free(plugin_name);
    free(install);
    return status;
}

