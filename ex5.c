/******************
Name: Davidi Hershkovitz
ID: 214165417
Assignment: ex5
*******************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Define the Song structure
typedef struct Song {
    char* title;
    char* artist;
    int year;
    char* lyrics;
    int streams;
} Song;

// Define the Playlist structure
typedef struct Playlist {
    char* name;
    Song** songs;
    int songsNum;
} Playlist;

char* readString();
void addPlaylist(Playlist*** playlists, int* playlistsNum);
void addSong(Playlist* playlist);
void showPlaylist(Playlist* playlist);
void playSongs(Playlist* playlist);
void sortPlaylist(Playlist* playlist);
void deleteSong(Playlist* playlist);
void playlistMenu(Playlist* playlist);
void viewPlaylists(Playlist** playlists, int playlistsNum);
void removePlaylist(Playlist*** playlists, int* playlistsNum);

char* readString() {
    char* str = malloc(sizeof(char));
    if (!str) exit(1);

    size_t size = 0, capacity = 1;
    char c;

    while ((c = getchar()) != '\n' && c != EOF) {
        if (size + 1 >= capacity) {
            capacity *= 2;
            char* temp = realloc(str, capacity * sizeof(char));
            if (!temp) {
                free(str);
                printf("Memory allocation failed\n");
                exit(1);
            }
            str = temp;
        }
        str[size++] = c;
    }
    str[size] = '\0';
    return str;
}

void addPlaylist(Playlist*** playlists, int* playlistsNum) {
    printf("Enter playlist's name:\n");
    char* name = readString();

    Playlist* newPlaylist = malloc(sizeof(Playlist));
    if (!newPlaylist) exit(1);

    newPlaylist->name = name;
    newPlaylist->songs = NULL;
    newPlaylist->songsNum = 0;

    Playlist** temp = realloc(*playlists, (*playlistsNum + 1) * sizeof(Playlist*));
    if (!temp) {
        free(newPlaylist->name);
        free(newPlaylist);
        printf("Memory allocation failed\n");
        exit(1);
    }
    *playlists = temp;
    (*playlists)[*playlistsNum] = newPlaylist;
    (*playlistsNum)++;
}

void addSong(Playlist* playlist) {
    printf("Enter song's details\n");
    printf("Title:\n");
    char* title = readString();
    printf("Artist:\n");
    char* artist = readString();
    printf("Year of release:\n");

    int year;
    if (scanf("%d", &year) != 1) {
        printf("Invalid input for year.\n");
        while (getchar() != '\n'); // Clear buffer
        return;
    }
    getchar(); // Clear newline
    printf("Lyrics:\n");
    char* lyrics = readString();

    Song* newSong = malloc(sizeof(Song));
    if (!newSong) {
        free(title);
        free(artist);
        free(lyrics);
        exit(1);
    }

    newSong->title = title;
    newSong->artist = artist;
    newSong->year = year;
    newSong->lyrics = lyrics;
    newSong->streams = 0;

    Song** temp = realloc(playlist->songs, (playlist->songsNum + 1) * sizeof(Song*));
    if (!temp) {
        free(newSong->title);
        free(newSong->artist);
        free(newSong->lyrics);
        free(newSong);
        printf("Memory allocation failed\n");
        exit(1);
    }
    playlist->songs = temp;
    playlist->songs[playlist->songsNum] = newSong;
    playlist->songsNum++;
}

void showPlaylist(Playlist* playlist) {
    if (playlist->songsNum == 0) {
        printf("Playlist %s is empty.\n", playlist->name);
        return;
    }

    for (int i = 0; i < playlist->songsNum; i++) {
        Song* song = playlist->songs[i];
        printf("%d. Title: %s\n   Artist: %s\n   Released: %d\n   Streams: %d\n",
               i + 1, song->title, song->artist, song->year, song->streams);
    }
}

void playSongs(Playlist* playlist) {
    if (playlist->songsNum == 0) {
        printf("Playlist %s is empty.\n", playlist->name);
        return;
    }

    for (int i = 0; i < playlist->songsNum; i++) {
        Song* song = playlist->songs[i];
        printf("Now playing %s:\n♪ %s ♪\n", song->title, song->lyrics);
        song->streams++;
    }
}

void sortPlaylist(Playlist* playlist) {
    if (playlist->songsNum == 0) {
        printf("Playlist %s is empty.\n", playlist->name);
        return;
    }

    printf("Choose:\n1. Sort by year\n2. Sort by streams ascending\n3. Sort by streams descending\n4. Sort alphabetically\n");

    int choice;
    if (scanf("%d", &choice) != 1) {
        printf("Invalid input.\n");
        while (getchar() != '\n'); // Clear buffer
        return;
    }
    getchar(); // Clear newline

    for (int i = 0; i < playlist->songsNum - 1; i++) {
        for (int j = i + 1; j < playlist->songsNum; j++) {
            int swap = 0;
            if (choice == 1 && playlist->songs[i]->year > playlist->songs[j]->year) swap = 1;
            if (choice == 2 && playlist->songs[i]->streams > playlist->songs[j]->streams) swap = 1;
            if (choice == 3 && playlist->songs[i]->streams < playlist->songs[j]->streams) swap = 1;
            if (choice == 4 && strcmp(playlist->songs[i]->title, playlist->songs[j]->title) > 0) swap = 1;

            if (swap) {
                Song* temp = playlist->songs[i];
                playlist->songs[i] = playlist->songs[j];
                playlist->songs[j] = temp;
            }
        }
    }
    printf("Sorted\n");
}

void deleteSong(Playlist* playlist) {
    if (playlist->songsNum == 0) {
        printf("Playlist %s is empty.\n", playlist->name);
        return;
    }

    showPlaylist(playlist);
    printf("Choose a song to delete, or 0 to quit:\n");

    int choice;
    if (scanf("%d", &choice) != 1 || choice < 0 || choice > playlist->songsNum) {
        printf("Invalid option\n");
        while (getchar() != '\n'); // Clear buffer
        return;
    }
    getchar(); // Clear newline

    if (choice == 0) return;

    Song* song = playlist->songs[choice - 1];
    free(song->title);
    free(song->artist);
    free(song->lyrics);
    free(song);

    for (int i = choice; i < playlist->songsNum; i++) {
        playlist->songs[i - 1] = playlist->songs[i];
    }

    playlist->songsNum--;
    playlist->songs = realloc(playlist->songs, playlist->songsNum * sizeof(Song*));
    printf("Song deleted successfully.\n");
}

void playlistMenu(Playlist* playlist) {
    while (1) {
        printf("Playlist %s:\n1. Show Playlist\n2. Add Song\n3. Delete Song\n4. Sort\n5. Play\n6. exit\n", playlist->name);
        int choice;
        if (scanf("%d", &choice) != 1) {
            printf("Invalid input.\n");
            while (getchar() != '\n'); // Clear buffer
            continue;
        }
        getchar(); // Clear newline

        switch (choice) {
            case 1: showPlaylist(playlist); break;
            case 2: addSong(playlist); break;
            case 3: deleteSong(playlist); break;
            case 4: sortPlaylist(playlist); break;
            case 5: playSongs(playlist); break;
            case 6: return;
            default: printf("Invalid option\n");
        }
    }
}

void viewPlaylists(Playlist** playlists, int playlistsNum) {
    while (1) {
        printf("Choose a playlist:\n");
        for (int i = 0; i < playlistsNum; i++) {
            printf("%d. %s\n", i + 1, playlists[i]->name);
        }
        printf("%d. Back to main menu\n", playlistsNum + 1);

        int choice;
        if (scanf("%d", &choice) != 1) {
            printf("Invalid input.\n");
            while (getchar() != '\n'); // Clear buffer
            continue;
        }
        getchar(); // Clear newline

        if (choice == playlistsNum + 1) return;

        if (choice >= 1 && choice <= playlistsNum) {
            playlistMenu(playlists[choice - 1]);
        } else {
            printf("Invalid option\n");
        }
    }
}

void removePlaylist(Playlist*** playlists, int* playlistsNum) {
    int choice;

    while (1) {
        printf("Choose a playlist:\n");

        if (*playlistsNum == 0) {
            printf("\t1. Back to main menu\n");
        } else {
            for (int i = 0; i < *playlistsNum; i++) {
                printf("\t%d. %s\n", i + 1, (*playlists)[i]->name);
            }
            printf("\t%d. Back to main menu\n", *playlistsNum + 1);
        }

        if (scanf("%d", &choice) != 1) {
            printf("Invalid input. Please enter a number.\n");
            while (getchar() != '\n'); // Clear invalid input
            continue;
        }
        getchar(); // Clear newline from buffer

        if (*playlistsNum == 0 && choice == 1) {
            return; // Back to main menu
        }

        if (*playlistsNum > 0) {
            if (choice == *playlistsNum + 1) {
                return; // Back to main menu
            }

            if (choice >= 1 && choice <= *playlistsNum) {
                Playlist* playlistToDelete = (*playlists)[choice - 1];
                free(playlistToDelete->name);
                for (int i = 0; i < playlistToDelete->songsNum; i++) {
                    free(playlistToDelete->songs[i]->title);
                    free(playlistToDelete->songs[i]->artist);
                    free(playlistToDelete->songs[i]->lyrics);
                    free(playlistToDelete->songs[i]);
                }
                free(playlistToDelete->songs);
                free(playlistToDelete);

                for (int i = choice; i < *playlistsNum; i++) {
                    (*playlists)[i - 1] = (*playlists)[i];
                }

                (*playlistsNum)--;

                Playlist** temp = realloc(*playlists, (*playlistsNum) * sizeof(Playlist*));
                if (*playlistsNum > 0 && !temp) {
                    printf("Memory allocation failed\n");
                    exit(1);
                }
                *playlists = temp;

                printf("Playlist deleted successfully.\n");
                return;
            }

            printf("Invalid option. Please choose a valid playlist number.\n");
        }
    }
}

int main() {
    Playlist** playlists = NULL;
    int playlistsNum = 0;
    int choice;

    while (1) {
        printf("Please Choose:\n\t1. Watch playlists\n\t2. Add playlist\n\t3. Remove playlist\n\t4. exit\n");

        if (scanf("%d", &choice) != 1) {
            printf("Invalid input. Please enter a valid number.\n");
            while (getchar() != '\n'); // Clear invalid input
            continue;
        }
        getchar(); // Clear newline

        switch (choice) {
        case 1:
            viewPlaylists(playlists, playlistsNum);
            break;
        case 2:
            addPlaylist(&playlists, &playlistsNum);
            break;
        case 3:
            removePlaylist(&playlists, &playlistsNum);
            break;
        case 4:
            // Free all memory before exiting
                for (int i = 0; i < playlistsNum; i++) {
                    free(playlists[i]->name);
                    for (int j = 0; j < playlists[i]->songsNum; j++) {
                        free(playlists[i]->songs[j]->title);
                        free(playlists[i]->songs[j]->artist);
                        free(playlists[i]->songs[j]->lyrics);
                        free(playlists[i]->songs[j]);
                    }
                    free(playlists[i]->songs);
                    free(playlists[i]);
                }
            free(playlists);
            printf("Goodbye!\n");
            return 0;
        default:
            printf("Invalid option\n");
        }
    }
}