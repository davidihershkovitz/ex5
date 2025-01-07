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

char* readPlaylistName();
char* readSongTitle();
char* readSongArtist();
char* readSongLyrics();
void addPlaylist(Playlist*** playlists, int* playlistsNum);
void addSong(Playlist* playlist);
void showPlaylist(Playlist* playlist);
void playSongs(Playlist* playlist);
void sortPlaylist(Playlist* playlist);
void deleteSong(Playlist* playlist);
void playlistMenu(Playlist* playlist);
void viewPlaylists(Playlist** playlists, int playlistsNum);
void removePlaylist(Playlist*** playlists, int* playlistsNum);


// Function to read a playlist name
char* readPlaylistName() {
    printf("Enter playlist's name:\n");
    char* name = malloc(sizeof(char));
    if (!name) exit(1);

    size_t size = 0, capacity = 1;
    char c;

    while ((c = getchar()) != '\n') {
        if (size + 1 >= capacity) {
            capacity *= 2; // Double the capacity
            char* temp = realloc(name, capacity * sizeof(char));
            if (!temp) {
                free(name); // Avoid memory leak
                printf("Memory allocation failed\n");
                exit(1);
            }
            name = temp;
        }
        name[size++] = c;
    }
    name[size] = '\0'; // Null-terminate the string
    return name;
}

// Function to read a song title
char* readSongTitle() {
    printf("Title:\n");
    char* title = malloc(sizeof(char));
    if (!title) exit(1); // Check initial allocation

    size_t size = 0, capacity = 1;
    char c;

    while ((c = getchar()) != '\n') {
        if (size + 1 >= capacity) {
            capacity *= 2; // Double capacity dynamically
            char* temp = realloc(title, capacity * sizeof(char));
            if (!temp) {
                free(title); // Avoid memory leak
                printf("Memory allocation failed\n");
                exit(1);
            }
            title = temp;
        }
        title[size++] = c;
    }
    title[size] = '\0'; // Null-terminate the string
    return title;
}
// Function to read a song's artist
char* readSongArtist() {
    printf("Artist:\n");
    char* artist = malloc(sizeof(char));
    if (!artist) exit(1); // Check initial allocation

    size_t size = 0, capacity = 1;
    char c;

    while ((c = getchar()) != '\n') {
        if (size + 1 >= capacity) {
            capacity *= 2; // Double capacity dynamically
            char* temp = realloc(artist, capacity * sizeof(char));
            if (!temp) {
                free(artist); // Avoid memory leak
                printf("Memory allocation failed\n");
                exit(1);
            }
            artist = temp; // Assign the new memory to artist
        }
        artist[size++] = c;
    }
    artist[size] = '\0'; // Null-terminate the string
    return artist;
}

// Function to read song lyrics
char* readSongLyrics() {
    printf("Lyrics:\n");
    char* lyrics = malloc(sizeof(char));
    if (!lyrics) exit(1); // Check initial allocation

    size_t size = 0, capacity = 1;
    char c;

    while ((c = getchar()) != '\n') {
        if (size + 1 >= capacity) {
            capacity *= 2; // Double capacity dynamically
            char* temp = realloc(lyrics, capacity * sizeof(char));
            if (!temp) {
                free(lyrics); // Free original memory to prevent leak
                printf("Memory allocation failed\n");
                exit(1);
            }
            lyrics = temp; // Assign the new memory to lyrics
        }
        lyrics[size++] = c;
    }
    lyrics[size] = '\0'; // Null-terminate the string
    return lyrics;
}

// Function to add a playlist
void addPlaylist(Playlist*** playlists, int* playlistsNum) {
    char* playlistName = readPlaylistName(); // Prompt already happens here

    Playlist* newPlaylist = malloc(sizeof(Playlist));
    if (!newPlaylist) exit(1);

    newPlaylist->name = playlistName;
    newPlaylist->songs = NULL;
    newPlaylist->songsNum = 0;

    *playlists = realloc(*playlists, (*playlistsNum + 1) * sizeof(Playlist*));
    if (!*playlists) exit(1);

    (*playlists)[*playlistsNum] = newPlaylist;
    (*playlistsNum)++;
}
// func that add songs
void addSong(Playlist* playlist) {
    Song* newSong = malloc(sizeof(Song));
    if (!newSong) exit(1);

    printf("Enter song's details\n");
    newSong->title = readSongTitle();
    newSong->artist = readSongArtist();

    printf("Year of release:\n");
    scanf("%d", &newSong->year);
    getchar(); // Clear newline

    newSong->lyrics = readSongLyrics();
    newSong->streams = 0;

    // Resize the playlist's songs array
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

    // Display all songs once
    for (int i = 0; i < playlist->songsNum; i++) {
        Song* song = playlist->songs[i];
        printf("%d. Title: %s\n   Artist: %s\n   Released: %d\n   Streams: %d\n\n",
               i + 1, song->title, song->artist, song->year, song->streams);
    }

    // Keep prompting the user to play songs
    while (1) {
        printf("choose a song to play, or 0 to quit:\n");

        int choice;
        scanf("%d", &choice);
        getchar(); // Clear newline

        if (choice == 0) {
            return; // Exit to playlist menu
        }
        if (choice >= 1 && choice <= playlist->songsNum) {
            // Play the selected song
            Song* song = playlist->songs[choice - 1];
            printf("Now playing %s:\n♪ %s ♪\n", song->title, song->lyrics);
            song->streams++; // Increment the streams counter
        } else {
            // Handle invalid input
            printf("Invalid option\n");
        }
    }
}

// Function to play songs
void playSongs(Playlist* playlist) {
    if (playlist->songsNum == 0) {
        printf("Playlist %s is empty.\n", playlist->name);
        return;
    }

    for (int i = 0; i < playlist->songsNum; i++) {
        Song* song = playlist->songs[i];
        printf("Now playing %s:\n♪ %s ♪\n\n", song->title, song->lyrics);
        song->streams++;
    }
}

// Function to sort the playlist
void sortPlaylist(Playlist* playlist) {
    if (playlist->songsNum == 0) {
        printf("Playlist %s is empty.\n", playlist->name);
        return;
    }

    printf("choose:\n");
    printf("1. sort by year\n");
    printf("2. sort by streams - ascending order\n");
    printf("3. sort by streams - descending order\n");
    printf("4. sort alphabetically\n");

    int choice;
    scanf("%d", &choice);
    getchar(); // Clear newline

    if (choice < 1 || choice > 4) {
        choice = 4; // Default to alphabetical sorting for invalid choice
    }

    for (int i = 0; i < playlist->songsNum - 1; i++) {
        for (int j = i + 1; j < playlist->songsNum; j++) {
            int swap = 0;

            if (choice == 1 && playlist->songs[i]->year > playlist->songs[j]->year) {
                swap = 1;
            } else if (choice == 2 && playlist->songs[i]->streams > playlist->songs[j]->streams) {
                swap = 1;
            } else if (choice == 3 && playlist->songs[i]->streams < playlist->songs[j]->streams) {
                swap = 1;
            } else if (choice == 4 && strcmp(playlist->songs[i]->title, playlist->songs[j]->title) > 0) {
                swap = 1;
            }

            if (swap) {
                Song* temp = playlist->songs[i];
                playlist->songs[i] = playlist->songs[j];
                playlist->songs[j] = temp;
            }
        }
    }

    printf("sorted\n");
}

void deleteSong(Playlist* playlist) {
    if (playlist->songsNum == 0) {
        printf("Playlist %s is empty.\n", playlist->name);
        return;
    }

    // Show the playlist songs
    for (int i = 0; i < playlist->songsNum; i++) {
        Song* song = playlist->songs[i];
        printf("%d. Title: %s\n   Artist: %s\n   Released: %d\n   Streams: %d\n",
               i + 1, song->title, song->artist, song->year, song->streams);
    }

    printf("choose a song to delete, or 0 to quit:\n");

    int choice;
    scanf("%d", &choice);
    getchar(); // Clear newline

    if (choice == 0) {
        return; // User chose to quit
    }

    if (choice < 1 || choice > playlist->songsNum) {
        printf("Invalid option\n");
        return;
    }

    // Free memory of the selected song
    Song* songToDelete = playlist->songs[choice - 1];
    free(songToDelete->title);
    free(songToDelete->artist);
    free(songToDelete->lyrics);
    free(songToDelete);

    // Shift the remaining songs
    for (int i = choice; i < playlist->songsNum; i++) {
        playlist->songs[i - 1] = playlist->songs[i];
    }

    playlist->songsNum--;

    // Resize the playlist's songs array
    Song** temp = realloc(playlist->songs, playlist->songsNum * sizeof(Song*));
    if (playlist->songsNum > 0 && !temp) {
        printf("Memory allocation failed\n");
        exit(1);
    }
    playlist->songs = temp;

    printf("Song deleted successfully.\n");
}

void playlistMenu(Playlist* playlist) {
    int choice;
    int firstTime = 1; // Flag to track first entry to the menu

    while (1) {
        if (firstTime) {
            // Print playlist name only on first entry
            printf("playlist %s:\n", playlist->name);
            firstTime = 0; // Set flag to avoid repeating the name
        }

        // Print the menu options
        printf("\t1. Show Playlist\n");
        printf("\t2. Add Song\n");
        printf("\t3. Delete Song\n");
        printf("\t4. Sort\n");
        printf("\t5. Play\n");
        printf("\t6. exit\n");

        // Get user choice
        scanf("%d", &choice);
        getchar(); // Clear newline

        switch (choice) {
        case 1:
            showPlaylist(playlist);
            break;
        case 2:
            addSong(playlist);
            break;
        case 3:
            deleteSong(playlist);
            break;
        case 4:
            sortPlaylist(playlist);
            break;
        case 5:
            playSongs(playlist);
            break;
        case 6:
            return; // Exit to the main menu
        default:
            printf("Invalid option\n");
        }
    }
}

void viewPlaylists(Playlist** playlists, int playlistsNum) {
    int choice;

    while (1) {
        printf("Choose a playlist:\n");

        if (playlistsNum == 0) {
            printf("\t1. Back to main menu\n");
        } else {
            for (int i = 0; i < playlistsNum; i++) {
                printf("\t%d. %s\n", i + 1, playlists[i]->name);
            }
            printf("\t%d. Back to main menu\n", playlistsNum + 1);
        }

        scanf("%d", &choice);
        getchar(); // Clear newline

        if (playlistsNum == 0 && choice == 1) {
            return; // Back to main menu
        }

        if (playlistsNum > 0) {
            if (choice == playlistsNum + 1) {
                return; // Back to main menu
            }
            if (choice >= 1 && choice <= playlistsNum) {
                // Enter the selected playlist menu
                playlistMenu(playlists[choice - 1]);
                continue; // Return to the loop after exiting the playlist menu
            }
        }

        printf("Invalid option\n");
    }
}
// Function to remove a playlist
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

        scanf("%d", &choice);
        getchar(); // Clear newline

        if (*playlistsNum == 0 && choice == 1) {
            return; // Back to main menu
        }

        if (*playlistsNum > 0) {
            if (choice == *playlistsNum + 1) {
                return; // Back to main menu
            }
            if (choice >= 1 && choice <= *playlistsNum) {
                // Delete the selected playlist
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

                // Shift remaining playlists
                for (int i = choice; i < *playlistsNum; i++) {
                    (*playlists)[i - 1] = (*playlists)[i];
                }

                (*playlistsNum)--;

                // Resize the playlists array
                *playlists = realloc(*playlists, (*playlistsNum) * sizeof(Playlist*));
                if (*playlistsNum > 0 && !*playlists) {
                    printf("malloc failed\n");
                    exit(1);
                }

                printf("Playlist deleted.\n");
                return; // Exit back to the main menu after deletion
            }
        }

        printf("Invalid option\n");
    }
}


int main() {
    Playlist** playlists = NULL;
    int playlistsNum = 0;
    int choice;

    while (1) {
        printf("Please Choose:\n\t1. Watch playlists\n\t2. Add playlist\n\t3. Remove playlist\n\t4. exit\n");

        scanf("%d", &choice);
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
