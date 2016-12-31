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
    strcat(plugin_path, getenv("HOME"));
    strcat(plugin_path, "/.zpm/plugins/");
    strcat(plugin_path, plugin_name);

    return plugin_path;
}

char* get_plugin_entry_point(char* plugin_name) {
    DIR *dir;
    struct dirent *ent;
    char* plugin_path = generate_plugin_path(plugin_name);
    char* plugin_entry_point = malloc(PATH_MAX);

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

    return plugin_entry_point;
}

char* get_zpmrc_path() {
    char* zpmrc = malloc(PATH_MAX);

    strcat(zpmrc, getenv("HOME"));
    strcat(zpmrc, "/.zpmrc");

    return zpmrc;
}

int generate_plugin_entry(char* plugin_name) {
    char* plugin_path = malloc(PATH_MAX);
    plugin_path = generate_plugin_path(plugin_name);
    char* plugin_entry = malloc(PATH_MAX);

    char* plugin_entry_point = get_plugin_entry_point(plugin_name);

    strcat(plugin_entry, "source ");
    strcat(plugin_entry, plugin_path);
    strcat(plugin_entry, "/");
    strcat(plugin_entry, plugin_entry_point);
    strcat(plugin_entry, "\n");

    char* zpmrc = malloc(PATH_MAX);
    zpmrc = get_zpmrc_path();
    FILE* store = fopen(zpmrc,"ab+");
	return fwrite(plugin_entry, strlen(plugin_entry), 1, store);
}

char* get_plugin_list_path() {
    char* plugin_list = malloc(PATH_MAX);

    strcpy(plugin_list, getenv("HOME"));
    strcat(plugin_list, "/.zpm/.plugin_list");

    return plugin_list;
}

int plugin_list_add_item(char* plugin_name) {
    char* plugin_item = malloc(PATH_MAX);
    strcpy(plugin_item, plugin_name);
    strcat(plugin_item, "\n");

    char* plugin_list = malloc(PATH_MAX);
    plugin_list = get_plugin_list_path();
    FILE* store = fopen(plugin_list,"ab+");
	return fwrite(plugin_item, strlen(plugin_item), 1, store);

}

int local_clone_exists(char* plugin_name) {
    char* plugin_path = generate_plugin_path(plugin_name);
    DIR* plugin_directory = opendir(plugin_path);
    if (plugin_directory) {
        closedir(plugin_directory);
    } else if (ENOENT == errno) {
        int status = mkdir(plugin_path, S_IRWXU);
        errno = status;
    }

    return errno;
}

int zpm_configuration_exists() {
    char* zpmrc_path = get_zpmrc_path();

	FILE *file;
	if ((file = fopen(zpmrc_path, "r")) == NULL) {
		return -1;
	} else {
		fclose(file);
	}

    return 0;
}

char* generate_repository_url(char* plugin_name) {
    char* url = malloc(PATH_MAX);
    strcat(url, "https://github.com/");
    strcat(url, plugin_name);

    return url;
}

int locally_clone_plugin(char* plugin_name) {
    char* repository_url = generate_repository_url(plugin_name);
    char* clone_destination = generate_plugin_path(plugin_name);

    char* command = malloc(PATH_MAX);
    strcat(command, "git clone --recursive --depth=1 ");
    strcat(command, repository_url);
    strcat(command, " ");
    strcat(command, clone_destination);
    strcat(command, " &> /dev/null\n");

    return system(command);
}

char* get_zpm_plugin_list() {
	char* plugin_list_path = get_plugin_list_path();
	char* listing = malloc(1024);
    FILE* list;

    list = fopen(plugin_list_path, "rb");
    fread(listing, 1, 1024, list);

    fclose(list);

    return listing;
}

int plugins_update_local_clone() {
    // iterate over plugins_list
    // exec git pull --git-dir=
    return 0;
}

int main(int argc, char* argv[]) {
    if (argc <= 1) {
        printf("%s\n", "Usage:\n\tzpm 'zsh-users/zsh-syntax-highlighting'");
        return 1;
    }

    char* plugin_name_or_command = argv[1];
    char* plugin_name = malloc(PATH_MAX);

    if (strstr(plugin_name_or_command, "reset")) {
		unlink(get_plugin_list_path());
        unlink(get_zpmrc_path());
        return 0;

    } else if (strstr(plugin_name_or_command, "update")) {
        plugins_update_local_clone();
        return 0;

    } else if (strstr(plugin_name_or_command, "list")) {
        printf("%s\n", get_zpm_plugin_list());
        return 0;

    } else {
        strcpy(plugin_name, plugin_name_or_command);
    }

    int status = 0;
    if (local_clone_exists(plugin_name)) {
        char* install = malloc(1024);
        strcpy(install, "Installing ");
        strcat(install, plugin_name);
        strcat(install, "...");
        printf("%s", install);

        status = locally_clone_plugin(plugin_name);
    }

    if (status == 0) {
        printf("%s\n", "Done.");
        generate_plugin_entry(plugin_name);
        plugin_list_add_item(plugin_name);
    } else {
        printf("%s\n", "Error!");
    }

    return status;
}

