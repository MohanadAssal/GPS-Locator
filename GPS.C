#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <float.h>

typedef struct {
    double ulat;
    double ulon;
    double ualt;
    double utime;
    char uname[100];
} user_t;

// Function to scan the user's data
user_t scan_user() {
    user_t user;
    printf("Enter Name, Latitude, Longitude, Altitude, and Time please: ");
    if (scanf("%s", user.uname) != 1) {
        printf("ERROR READING NAME\n");
        exit(1);
    }
    if (scanf("%lf", &user.ulat) != 1) {
        printf("ERROR READING LATITUDE\n");
        exit(1);
    }
    if (scanf("%lf", &user.ulon) != 1) {
        printf("ERROR READING LONGITUDE\n");
        exit(1);
    }
    if (scanf("%lf", &user.ualt) != 1) {
        printf("ERROR READING ALTITUDE\n");
        exit(1);
    }
    if (scanf("%lf", &user.utime) != 1) {
        printf("ERROR READING TIME\n");
        exit(1);
    }
    printf("\n");
    return user;
}

// Function that calculates the differences in latitude, longitude, and altitude
double calculate_distance(user_t user1, user_t user2) {
    return sqrt(pow(user1.ulat - user2.ulat, 2) +
                pow(user1.ulon - user2.ulon, 2) +
                pow(user1.ualt - user2.ualt, 2));
}

// Function that calculates the closest user to ours
user_t find_closest_user(user_t our_user, user_t *other_users, int num_users) {
    double min_distance = DBL_MAX;
    int closest_idx = -1;

    for (int i = 0; i < num_users; i++) {
        double distance = calculate_distance(our_user, other_users[i]);
        if (distance < min_distance) {
            min_distance = distance;
            closest_idx = i;
        }
    }

    return other_users[closest_idx];
}
struct distant {
  double distance;
  char name[1000];

};
// Our Main Program that finds the name and position of the closest user
#define MAX_USERS 100

void read_users_from_file(const char *filename, user_t users[]) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        perror("Error opening file");
        exit(1);
    }

    int num_users;
    if (fscanf(file, "%d", &num_users) != 1) {
        printf("Error reading number of users or number exceeds maximum");
        exit(1);
    }

    for (int i = 0; i < num_users; i++) {
        if (fscanf(file, "%lf %lf %lf %lf %s",
                   &users[i].ulat,
                   &users[i].ulon,
                   &users[i].ualt,
                   &users[i].utime,
                   users[i].uname) != 5) {
            printf("Error reading user data from the file.\n");
            exit(1);
        }
    }

    fclose(file);
}

int update(const char* filename) {
    FILE* file = fopen(filename, "r");

  if (file == NULL) {
      printf("Error opening file.\n");
      exit(1);
  }

    int numUsers;
  if (fscanf(file, "%d", &numUsers) != 1) 
  {
      printf("Error reading the number of users from the file.\n");
      exit(1);
  }
    fclose(file);

    return numUsers;
}
int main() {
    user_t our_user = scan_user();


    int num_users;

    printf("Enter the number of other users (maximum %d):\n", MAX_USERS);
    scanf("%d", &num_users);

    if (num_users > MAX_USERS) {
        printf("Number of users exceeds the maximum limit of %d.\n", MAX_USERS);
        return 1;
    }
  int read_file;
  printf("Do you want to fill other users from file?(1/0)\n");
  scanf("%d",&read_file);

   user_t other_users[MAX_USERS];
  if(read_file==1)
  {
    char file[100000];
    printf("Enter the path of the file: "); 

    if (scanf("%s", file)!= 1) {
        printf("Error in reading the file path.\n");
        exit(1);
    }

    num_users = update(file);

    read_users_from_file(file, other_users);


  }

   else 
    for (int i = 0; i < num_users; i++) {
        other_users[i] = scan_user();
    }

  struct distant save[num_users];
//array of structure to store the distance and its name 
  for(int i=0; i<num_users; i++)
    {
      save[i].distance = calculate_distance(our_user, other_users[i]);

      strcpy(save[i].name, other_users[i].uname);
    }

    user_t closest_user = find_closest_user(our_user, other_users, num_users);

  printf("Reference User: \nUser Me: %s\nLatitude  %.2lf\nlongtitude  %.2lf\nAltitude  %.2lf\n",our_user.uname,our_user.ulat,our_user.ulon, our_user.ualt);

  printf("List of other users:\n");

  for(int i=0; i<num_users ;i++)
    printf("\nUser %s:\nLatitude  %.2lf\nlongtitude  %.2lf\nAltitude  %.2lf\n",other_users[i].uname,other_users[i].ulat,other_users[i].ulon, other_users[i].ualt);

  printf("Actual distances from our user were:\n");
  for(int i=0; i<num_users; i++)
    printf("User number: %d at distance %.4lf named %s\n", i , save[i].distance,save[i].name);

  printf("Nearest user to reference user:\nUser %s:\n Latitude: %.2lf\nLongtitude: %.2lf\nAltitude: %.2lf\n",closest_user.uname, closest_user.ulat,closest_user.ulon, closest_user.ualt);
   // printf("Closest user: %s\n", closest_user.uname);


    return 0;
}
