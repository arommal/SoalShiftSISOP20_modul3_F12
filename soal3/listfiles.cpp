#include <sys/types.h>
#include <dirent.h>
#include <errno.h>
#include <vector>
#include <string>
#include <string.h>
#include <iostream>

using namespace std;

// getdir - returns vector of files in all directories of directory
int getdir(string dir, vector<string> &files)
{
  // directory pointer
  DIR *dp;
  // dirent struct for directory
  struct dirent *dirp;
  // attempt to open directory
  if((dp  = opendir(dir.c_str())) == NULL)
  {
    cout << "Error(" << errno << ") opening " << dir << endl;
    return errno;
  }

  // Loop through each file in the directory
  while ((dirp = readdir(dp)) != NULL)
  {
    // Recurse into subdirectory if found
    if (dirp->d_type == DT_DIR && strcmp(dirp->d_name,".") != 0 && strcmp(dirp->d_name,"..") != 0)
    {
      string p = dir + "/" + dirp->d_name;
      getdir(p, files);
    }
    // Found a hidden file, skip it
    else if (dirp->d_type == DT_REG && string(dirp->d_name).substr(0, 1) == ".")
    {
      continue;
    }
    // Add file to list
    else if (dirp->d_type == DT_REG)
    {
      files.push_back(string(dirp->d_name));
    }
  }
  // Close directory pointer
  closedir(dp);
  return 0;
}

int main(int argc, char *argv[])
{
  // Directory to list
  string dir = string(argv[1]);
  // Vector of strings to hold filenames
  vector<string> files = vector<string>();

  // getdir to list the files
  getdir(dir,files);

  // Loop through the results
  for (unsigned int i = 0;i < files.size();i++)
  {
    cout << files[i] << endl;
  }
  return 0;
}