/**
 * @file CheckDependency.c
 * @author Itai Tagar <itagar>
 * @version 1.7
 * @date 17 Aug 2016
 *
 * @brief A program that determines if a given file declares a cyclic dependency.
 *
 * @section LICENSE
 * This program is free to use in every operation system.
 *
 * @section DESCRIPTION
 * A program that determines if a given file declares a cyclic dependency.
 * Input:       A name or a path to a text file.
 * Process:     Validates input, if the input is valid the program starts to analyze the text file
 *              for determine if there is a cyclic dependency.
 *              It first process all the data in the given file, and store it in the memory, then
 *              it uses all the processed data to analyze the dependency.
 *              If the file is invalid the program ends with an error message.
 *              Algorithm Description:
 *              In order to determine cyclic dependency, the program use the DFS Algorithm.
 *              The program stores in an array all the Files that appeared in the file.
 *              Each File is a struct which holds it's dependencies.
 *              Using this array of Files, the algorithm can use DFS as learned in DaSt lessons.
 *              We create another 2 arrays - 'visited' and 'current'.
 *              If a File is in 'visited' it means we visited it and also all of it's neighbours,
 *              so all the Files in 'visited' are irrelevant for the continue of the Algorithm.
 *              If the File we are currently called DFS on is in 'current' array, we can
 *              determine that there is a cycle and finish the run.
 * Output:      A message that states the cyclic dependency result, if the input was valid.
 *              An error message in case of bad input.
 */


/*-----=  Includes  =-----*/


#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>


/*-----=  Definitions  =-----*/


/**
 * @def VALID_STATE 0
 * @brief A Flag for valid state during the program run.
 */
#define VALID_STATE 0

/**
 * @def INVALID_STATE -1
 * @brief A Flag for invalid state during the program run.
 */
#define INVALID_STATE -1

/**
 * @def VALID_ARGUMENTS_NUMBER 2
 * @brief A Macro that sets the valid number of arguments for this program.
 */
#define VALID_ARGUMENTS_NUMBER 2

/**
 * @def FILE_NAME_INDEX 1
 * @brief A Macro that sets the index of the File name in the arguments array.
 */
#define FILE_NAME_INDEX 1

/**
 * @def READ_MODE "r"
 * @brief A Flag represents the Read Mode when opening a file stream.
 */
#define READ_MODE "r"

/**
 * @def INVALID_ARGUMENTS_MESSAGE "Please supply one file!\nusage: CheckDependency <filename>\n"
 * @brief A Macro that sets the output message for invalid arguments.
 */
#define INVALID_ARGUMENTS_MESSAGE "Please supply one file!\nusage: CheckDependency <filename>\n"

/**
 * @def INVALID_FILE_ARGUMENTS_MESSAGE "Error! trying to open the file %s\n"
 * @brief A Macro that sets the output message for an invalid File argument.
 */
#define INVALID_FILE_ARGUMENTS_MESSAGE "Error! trying to open the file %s\n"

/**
 * @def DEPENDENCY_MESSAGE "Cyclic dependency\n"
 * @brief A Macro that sets the output message for cyclic dependency state.
 */
#define DEPENDENCY_MESSAGE "Cyclic dependency\n"

/**
 * @def NO_DEPENDENCY_MESSAGE "No Cyclic dependency\n"
 * @brief A Macro that sets the output message for no cyclic dependencies state.
 */
#define NO_DEPENDENCY_MESSAGE "No Cyclic dependency\n"

/**
 * @def MAX_LINE_SIZE 1001
 * @brief A Macro that sets the maximum number of characters in a single line.
 */
#define MAX_LINE_SIZE 1001

/**
 * @def MAX_NUMBER_OF_DEPENDENCIES 100
 * @brief A Macro that sets the maximum number of dependencies for a single file.
 */
#define MAX_NUMBER_OF_DEPENDENCIES 100

/**
 * @def INITIAL_INDEX 0
 * @brief A Macro that sets the initial index in the File's Array.
 */
#define START_INDEX 0

/**
 * @def FILE_NOT_FOUND -1
 * @brief A Flag for file not found value during search in an array.
 */
#define FILE_NOT_FOUND -1

/**
 * @def NO_DEPENDENCIES 0
 * @brief A Macro that sets the number of dependencies for a file with no dependencies.
 */
#define NO_DEPENDENCIES 0

/**
 * @def FILE_NAME_DELIMITER ": \n"
 * @brief A Flag for the file name delimiter in the input file.
 */
#define FILE_NAME_DELIMITER ": \n"

/**
 * @def FIRST_DEPENDENCY_DELIMITER " ,\n"
 * @brief A Flag for the first dependency delimiter in the input file.
 */
#define FIRST_DEPENDENCY_DELIMITER " ,\n"

/**
 * @def DEPENDENCY_DELIMITER ",\n"
 * @brief A Flag for the dependency delimiter in the input file.
 */
#define DEPENDENCY_DELIMITER ",\n"

/**
 * @def STRING_EQUALITY 0
 * @brief A Flag for the result of strings equality.
 */
#define STRING_EQUALITY 0

/**
 * @def STRING_TERMINATOR_COUNT 1
 * @brief A Macro that sets the number for the string terminator addition.
 */
#define STRING_TERMINATOR_COUNT 1

/**
 * @def NO_FILES 0
 * @brief A Macro that sets the number for no files.
 */
#define NO_FILES 0


/*-----=  Structs  =-----*/


/**
 * A Structure for a file in the program. It holds the name of the file, and it's dependencies as
 * well as the number of dependencies for this file.
 */
typedef struct File
{
    char *fileName;
    int fileDependencies[MAX_NUMBER_OF_DEPENDENCIES];
    int numberOfDependencies;
} File;


/*-----=  Forward Declarations  =-----*/


/**
 * @brief An estimate count for the number of files appears in the input file to read from.
 *        This is only an estimation and some files could be counted more then once.
 * @param pFile The input file containing the data.
 * @return The estimate count number of files.
 */
static int estimateFilesCount(FILE * const pFile);

/**
 * @brief Perform the first analysis of the input file. The function parse each line into the
 *        desired tokens. It process the data and create the proper File structs with the
 *        processed data, and store it in the general dependencies array.
 *        The function also analyze the File's dependencies and updates them properly.
 *        By the end of this function run, we have created a File struct for each file we have
 *        encountered, and each one of this File structs stores it's dependencies as desired.
 * @param pFile The input file to read.
 * @param dependencies The general dependencies array which holds the File structs.
 */
static void processFile(FILE * const pFile, File *dependencies);

/**
 * @brief Analyze the data that has been processed from the input file.
 *        Using the DFS Algorithm, this function determines if there is a cyclic dependency
 *        or not.
 * @param dependencies The general dependencies array which holds the File structs.
 * @return true if there is a cyclic dependency, false otherwise.
 */
static bool analyzeData(File * const dependencies);

/**
 * @brief Runs a DFS Algorithm on a given file.
 * @param currentFile The pointer for the current file to run DFS on.
 * @param dependencies The general dependencies array which holds the File structs.
 * @param currentDependencies The currently visited files.
 * @param visited All the files that were absolutely visited during the entire program run,
 *        i.e. a file in this array is a file which was visited and also all of its neighbours
 *        were visited.
 * @param currentIndex The current available index in the currentDependencies array.
 * @return true if there is a cyclic dependency, false otherwise.
 */
static bool dfs(File * const currentFile, File * const dependencies, File *currentDependencies,
                File *visited, int *currentIndex);

/**
 * @brief Determine if the given file name appears in the given dependencies array.
 *        This function used in the DFS Algorithm.
 * @param fileName The file name to check.
 * @param dependencies The array to check in.
 * @param count The current amount of files in the dependencies.
 * @return true if there is a cyclic dependency, false otherwise.
 */
static bool dfsContains(char const *fileName, File * const dependencies, int count);

/**
 * @brief With the given results from the analysis process, this function perform the required
 *        actions in each scenario (cyclic or no cyclic dependency).
 * @param result A boolean variable holding the result from the analysis process.
 */
static void analyzeResult(bool const result);

/**
 * @brief Handles the entire process of receiving the input from the user and process it.
 *        The function verifies a valid number of arguments, and that the given arguments are
 *        valid as well (i.e. valid input file).
 *        The function returns a pointer to the input file which was as an argument.
 * @param argc The number of given arguments.
 * @param argv[] The arguments from the user.
 * @return A pointer to the input file which was as an argument if the input is valid,
 *         otherwise returns NULL pointer.
 */
static FILE * handleInput(int argc, char *argv[]);

/**
 * @brief Free the memory of given several Files.
 * @param dependencies The pointer to the Files to free.
 */
static void freeAllFiles(File * const dependencies);

/**
 * @brief Initialize a given File struct with the desired parameters.
 * @param file A pointer to the File struct to initialize.
 * @param fileName The name of the File.
 * @param numberOfDependencies The number of dependencies of the given File.
 * @return true iff succeed.
 */
static bool initializeFile(File *file, char const *fileName, int const numberOfDependencies);

/**
 * @brief Counts the amount of dependencies of a file in a given line.
 * @param currentLine The current line to check.
 * @return The number of dependencies the file in the given line has.
 */
static int countDependencies(char *currentLine);

/**
 * @brief Creating and adding a new File struct instance to the general dependencies array.
 * @param fileName The file name for the File struct.
 * @param dependencies The general dependencies array which stores the Files.
 * @param numberOfDependencies The number of dependencies of the new File.
 * @return The index of the new File in the dependencies array.
 */
static int addNewFile(char const *fileName, File dependencies[], int const numberOfDependencies);

/**
 * @brief Updates a given File with new number of dependencies and allocating new memory for it's
 *        dependencies.
 * @param file The File to update.
 * @param numberOfDependencies The number of dependencies to add to the current count.
 */
static void updateExistingFile(File *file, int const numberOfDependencies);

/**
 * @brief Adds new dependency to a given File if it does not exists already. The new dependency is
 *        represented as an integer, which states the index of it in the general dependencies
 *        array.
 * @param file A pointer to the File we are about to update.
 * @param fileIndex The file index representing the new dependency file.
 * @param indexToStore The current available index by order in the File's dependencies
 *                     array to insert the new dependency.
 */
static void addNewDependency(File *file, int const fileIndex, int *indexToStore);

/**
 * @brief Determine if a given dependency file index has already exists as a dependency of the
 *        given file pointer.
 * @param file A pointer to the File contains dependencies.
 * @param fileIndex The file index representing the dependency file.
 * @param index The first index not occupied in the file dependencies array.
 * @return true if the dependency file exists, false otherwise.
 */
static bool containsDependency(File * const file, int const fileIndex, int const index);

/**
 * @brief Determine if a given file name has already appears in the File's dependencies array.
 *        If the file exists, the function returns it's index in the array, otherwise the function
 *        returns -1
 * @param fileName The file name to check.
 * @param dependencies The array which contains the existing Files.
 * @return The index of the given file name in the array if exists, otherwise returns -1.
 */
static int containsFile(char const *fileName, File * const dependencies);


/*-----=  Global Variables  =-----*/


int gNumberOfFiles = NO_FILES;


/*-----=  Main  =-----*/


/**
 * @brief The main function that runs the program.
 *        The function receive file path as input from the user, and analyze the file's data.
 *        If the user input is invalid, the program ends and return -1;
 *        The function determine if there is a cyclic dependency in the given file or not,
 *        and prints an informative message to the standard output.
 * @param argc The number of given arguments.
 * @param argv[] The arguments from the user.
 * @return 0 if the program run ended successfully, -1 if an error occurred.
 */
int main(int argc, char *argv[])
{
    FILE * pFile = handleInput(argc, argv);

    if (pFile == NULL)
    {
        return INVALID_STATE;
    }
    else
    {
        File *dependencies = NULL;

        // An estimate number of files, not accurate but it's used only to serve the memory
        // allocation for 'dependencies'.
        int const countEstimate = estimateFilesCount(pFile);
        assert(countEstimate >= NO_FILES);
        dependencies = (File *)malloc(countEstimate * sizeof(File));

        // Process the data of the file and close it's stream.
        processFile(pFile, dependencies);
        fclose(pFile);

        // Determine if there is Cyclic Dependency, and free all allocated memory.
        bool const result = analyzeData(dependencies);
        analyzeResult(result);

        // Free Memory.
        freeAllFiles(dependencies);
        free(dependencies);

        return VALID_STATE;
    }
}


/*-----=  Analyze Input File  =-----*/


/**
 * @brief An estimate count for the number of files appears in the input file to read from.
 *        This is only an estimation and some files could be counted more then once.
 * @param pFile The input file containing the data.
 * @return The estimate count number of files.
 */
static int estimateFilesCount(FILE * const pFile)
{
    assert(pFile != NULL);
    int counter = NO_FILES;
    char currentLine[MAX_LINE_SIZE] = {};
    while (fgets(currentLine, MAX_LINE_SIZE, pFile) != NULL)
    {
        counter++;  // A new line declare a new file (not always but it is an estimate count).
        int numberOfDependencies = countDependencies(currentLine);
        counter += numberOfDependencies;
    }
    rewind(pFile);  // Resets the file to the beginning for the next functions use.
    return counter;
}

/**
 * @brief Perform the first analysis of the input file. The function parse each line into the
 *        desired tokens. It process the data and create the proper File structs with the
 *        processed data, and store it in the general dependencies array.
 *        The function also analyze the File's dependencies and updates them properly.
 *        By the end of this function run, we have created a File struct for each file we have
 *        encountered, and each one of this File structs stores it's dependencies as desired.
 * @param pFile The input file to read.
 * @param dependencies The general dependencies array which holds the File structs.
 */
static void processFile(FILE * const pFile, File *dependencies)
{
    // The current line read from the input file.
    char currentLine[MAX_LINE_SIZE] = {};

    // Analyzing each line in the file.
    while (fgets(currentLine, MAX_LINE_SIZE, pFile) != NULL)
    {
        // Each new line we initialize these variables which helps us with processing the line.
        char currentLineCopy[MAX_LINE_SIZE] = {};
        File *currentFile = NULL;
        char *fileName = NULL;
        char *fileDependency = NULL;
        int index = START_INDEX;

        // Count the number of dependencies in this current line.
        // We copy the current line in order to perform manipulations on it without changing it.
        strcpy(currentLineCopy, currentLine);
        int const numberOfDependencies = countDependencies(currentLineCopy);
        assert(numberOfDependencies >= NO_DEPENDENCIES);

        // File name process.
        fileName = strtok(currentLine, FILE_NAME_DELIMITER);
        // Determine if the current file is already exists in the dependencies array.
        // If the file does not exist, we create a new File struct and store it in the
        // dependencies array.
        // If the file exists, we find it in the dependencies array and update it's current number
        // of dependencies.
        int currentFileIndex = containsFile(fileName, dependencies);
        if (currentFileIndex == FILE_NOT_FOUND)
        {
            currentFileIndex = addNewFile(fileName, dependencies, numberOfDependencies);
            currentFile = &(dependencies[currentFileIndex]);
        }
        else
        {
            currentFile = &(dependencies[currentFileIndex]);
            assert(currentFile != NULL);
            assert(currentFile -> numberOfDependencies >= NO_DEPENDENCIES);
            index = currentFile -> numberOfDependencies;
            updateExistingFile(currentFile, numberOfDependencies);
        }

        // File's dependencies process.
        fileDependency = strtok(NULL, FIRST_DEPENDENCY_DELIMITER);
        while (fileDependency != NULL)
        {
            // If we encountered a file which does not exists already, we create for it a new
            // File struct and store it in the dependencies array.
            int currentDependencyIndex = containsFile(fileDependency, dependencies);
            if (currentDependencyIndex == FILE_NOT_FOUND)
            {
                currentDependencyIndex = addNewFile(fileDependency, dependencies, NO_DEPENDENCIES);
            }

            // Assign the index of the file from the general dependencies array into the current
            // file's dependencies.
            addNewDependency(currentFile, currentDependencyIndex, &index);

            fileDependency = strtok(NULL, DEPENDENCY_DELIMITER);
        }
    }
    return;
}

/**
 * @brief Analyze the data that has been processed from the input file.
 *        Using the DFS Algorithm, this function determines if there is a cyclic dependency
 *        or not.
 * @param dependencies The general dependencies array which holds the File structs.
 * @return true if there is a cyclic dependency, false otherwise.
 */
static bool analyzeData(File * const dependencies)
{
    File *currentFile = NULL;
    File *visited = NULL;
    visited = (File *)malloc(gNumberOfFiles * sizeof(File));
    assert(visited != NULL);

    int i;
    for (i = 0 ; i < gNumberOfFiles; i++)
    {
        currentFile = &dependencies[i];
        int currentIndex = START_INDEX - 1;
        File *currentDependencies = NULL;
        currentDependencies = (File *)malloc(gNumberOfFiles * sizeof(File));
        assert(currentDependencies != NULL);

        // Run DFS Algorithm.
        if (dfs(currentFile, dependencies, currentDependencies, visited, &currentIndex))
        {
            free(currentDependencies);
            free(visited);
            return true;
        }

        free(currentDependencies);

    }
    free(visited);
    return false;
}

/**
 * @brief Runs a DFS Algorithm on a given file.
 * @param currentFile The pointer for the current file to run DFS on.
 * @param dependencies The general dependencies array which holds the File structs.
 * @param currentDependencies The currently visited files.
 * @param visited All the files that were absolutely visited during the entire program run,
 *        i.e. a file in this array is a file which was visited and also all of its neighbours
 *        were visited.
 * @param currentIndex The current available index in the currentDependencies array.
 * @return true if there is a cyclic dependency, false otherwise.
 */
static bool dfs(File * const currentFile, File * const dependencies, File *currentDependencies,
                File *visited, int *currentIndex)
{
    assert(currentFile != NULL);
    assert(dependencies != NULL);
    assert(visited != NULL);
    assert(currentIndex != NULL);

    File *fileDependency = NULL;
    static int visitedIndex = START_INDEX - 1;

    int i;
    for (i = 0 ; i < (currentFile -> numberOfDependencies); i++)
    {
        // Assigning the next dependency of currentFile.
        fileDependency = &(dependencies[currentFile -> fileDependencies[i]]);
        char *fileName = fileDependency -> fileName;

        assert(fileName != NULL);
        // In case the dependency file has already been fully visited, i.e. we visited it and all
        // of it's neighbours, we can ignore it and skip to the next dependency file.
        if (dfsContains(fileName, visited, visitedIndex))
        {
            continue;
        }
        // In case dependency file is in the current dependencies array, it means we found a cycle.
        else if (dfsContains(fileName, currentDependencies, *currentIndex))
        {
            return true;
        }
        // Otherwise, we add the current dependency file to the current dependencies array.
        else
        {
            assert(fileDependency != NULL);
            currentDependencies[++(*currentIndex)] = *fileDependency;
        }

        // We recursively call for DFS with the current dependency file.
        if (dfs(fileDependency, dependencies, currentDependencies, visited, currentIndex))
        {
            return true;
        }
    }

    // After visiting all of currentFile neighbours, we add it to the visited array if needed.
    assert((currentFile -> fileName) != NULL);
    if (!(dfsContains(currentFile -> fileName, visited, visitedIndex)))
    {
        visited[++visitedIndex] = *currentFile;
    }

    return false;
}

/**
 * @brief Determine if the given file name appears in the given dependencies array.
 *        This function used in the DFS Algorithm.
 * @param fileName The file name to check.
 * @param dependencies The array to check in.
 * @param count The current amount of files in the dependencies.
 * @return true if there is a cyclic dependency, false otherwise.
 */
static bool dfsContains(char const *fileName, File * const dependencies, int count)
{
    assert(fileName != NULL);
    assert(dependencies != NULL);

    while (count >= START_INDEX)
    {
        assert(dependencies[count].fileName != NULL);
        if (strcmp(dependencies[count].fileName, fileName) == STRING_EQUALITY)
        {
            return true;
        }
        count--;
    }
    return false;
}

/**
 * @brief With the given results from the analysis process, this function perform the required
 *        actions in each scenario (cyclic or no cyclic dependency).
 * @param result A boolean variable holding the result from the analysis process.
 */
static void analyzeResult(bool const result)
{
    if (result)
    {
        printf(DEPENDENCY_MESSAGE);
    }
    else
    {
        printf(NO_DEPENDENCY_MESSAGE);
    }
}


/*-----=  Input Handling  =-----*/


/**
 * @brief Handles the entire process of receiving the input from the user and process it.
 *        The function verifies a valid number of arguments, and that the given arguments are
 *        valid as well (i.e. valid input file).
 *        The function returns a pointer to the input file which was as an argument.
 * @param argc The number of given arguments.
 * @param argv[] The arguments from the user.
 * @return A pointer to the input file which was as an argument if the input is valid,
 *         otherwise returns NULL pointer.
 */
static FILE * handleInput(int argc, char *argv[])
{
    // Check valid arguments.
    if (argc != VALID_ARGUMENTS_NUMBER)
    {
        fprintf(stderr, INVALID_ARGUMENTS_MESSAGE);
        return NULL;
    }
    else
    {
        // Receive the File to check.
        FILE *pFile = NULL;
        pFile = fopen(argv[FILE_NAME_INDEX], READ_MODE);

        // In case of bad file.
        if (pFile == NULL)
        {
            fprintf(stderr, INVALID_FILE_ARGUMENTS_MESSAGE, argv[FILE_NAME_INDEX]);
        }

        return pFile;
    }
}


/*-----=  File Struct Handling  =-----*/


/**
 * @brief Free the memory of a given File.
 * @param file The File to free it's memory.
 */
static void freeFile(File *file)
{
    if (file != NULL)
    {
        free(file -> fileName);
        file -> fileName = NULL;
    }
    return;
}

/**
 * @brief Free the memory of given several Files.
 * @param dependencies The pointer to the Files to free.
 */
static void freeAllFiles(File * const dependencies)
{
    if (dependencies != NULL)
    {
        int i;
        for (i = 0 ; i < gNumberOfFiles; i++)
        {
            freeFile(&dependencies[i]);
        }
    }
    return;
}

/**
 * @brief Initialize a given File struct with the desired parameters.
 * @param file A pointer to the File struct to initialize.
 * @param fileName The name of the File.
 * @param numberOfDependencies The number of dependencies of the given File.
 * @return true iff succeed.
 */
static bool initializeFile(File *file, char const *fileName, int const numberOfDependencies)
{
    assert(file != NULL);
    assert(fileName != NULL);
    assert(numberOfDependencies >= NO_DEPENDENCIES);

    // Initialize File Name.
    file -> fileName = (char *)malloc(strlen(fileName) + STRING_TERMINATOR_COUNT);
    if (file -> fileName == NULL)
    {
        return false;
    }
    memcpy(file -> fileName, fileName, strlen(fileName) + STRING_TERMINATOR_COUNT);

    // Initialize File Dependency.
    file -> numberOfDependencies = numberOfDependencies;
    return true;
}

/**
 * @brief Counts the amount of dependencies of a file in a given line.
 * @param currentLine The current line to check.
 * @return The number of dependencies the file in the given line has.
 */
static int countDependencies(char *currentLine)
{
    assert(currentLine != NULL);
    char *fileDependency = NULL;
    int numberOfDependencies = NO_DEPENDENCIES;
    strtok(currentLine, FILE_NAME_DELIMITER);  // Skip the file's name token.

    fileDependency = strtok(NULL, FIRST_DEPENDENCY_DELIMITER);
    while (fileDependency != NULL)
    {
        // Only if after the ',' delimiter appears a valid name, we update the dependencies number.
        if (isalpha(fileDependency[START_INDEX]) || isdigit(fileDependency[START_INDEX]))
        {
            numberOfDependencies++;
        }
        fileDependency = strtok(NULL, DEPENDENCY_DELIMITER);
    }

    return numberOfDependencies;
}

/**
 * @brief Creating and adding a new File struct instance to the general dependencies array.
 * @param fileName The file name for the File struct.
 * @param dependencies The general dependencies array which stores the Files.
 * @param numberOfDependencies The number of dependencies of the new File.
 * @return The index of the new File in the dependencies array.
 */
static int addNewFile(char const *fileName, File dependencies[], int const numberOfDependencies)
{
    // The last index in the dependencies array which contains a File struct.
    static int lastFileIndex = START_INDEX;

    // Initialize the File struct.
    File file = {NULL, {FILE_NOT_FOUND}, NO_DEPENDENCIES};
    initializeFile(&file, fileName, numberOfDependencies);

    // Insert the new File struct to the proper place in the dependencies array.
    assert(dependencies != NULL);
    dependencies[lastFileIndex] = file;
    gNumberOfFiles++;
    return lastFileIndex++;
}

/**
 * @brief Updates a given File with new number of dependencies and allocating new memory for it's
 *        dependencies.
 * @param file The File to update.
 * @param numberOfDependencies The number of dependencies to add to the current count.
 */
static void updateExistingFile(File *file, int const numberOfDependencies)
{
    // Updating the File's data.
    assert(file != NULL);
    file -> numberOfDependencies += numberOfDependencies;
}

/**
 * @brief Adds new dependency to a given File if it does not exists already. The new dependency is
 *        represented as an integer, which states the index of it in the general dependencies
 *        array.
 * @param file A pointer to the File we are about to update.
 * @param fileIndex The file index representing the new dependency file.
 * @param indexToStore The current available index by order in the File's dependencies
 *                     array to insert the new dependency.
 * @param index The first index not occupied in the file dependencies array.
 */
static void addNewDependency(File *file, int const fileIndex, int *indexToStore)
{
    assert(file != NULL);
    if (!(containsDependency(file, fileIndex, *indexToStore)))
    {
        assert(file -> fileDependencies != NULL);
        file -> fileDependencies[*indexToStore] = fileIndex;
        (*indexToStore)++;
    }
    else
    {
        // If the dependency is already exists, it was counted twice so we need to update properly.
        assert((file -> numberOfDependencies) >= NO_DEPENDENCIES);
        file -> numberOfDependencies--;
    }
    return;
}

/**
 * @brief Determine if a given dependency file index has already exists as a dependency of the
 *        given file pointer.
 * @param file A pointer to the File contains dependencies.
 * @param fileIndex The file index representing the dependency file.
 * @param index The first index not occupied in the file dependencies array.
 * @return true if the dependency file exists, false otherwise.
 */
static bool containsDependency(File * const file, int const fileIndex, int const index)
{
    assert(file != NULL);

    int i;
    for (i = 0; i < index; i++)
    {
        if ((file -> fileDependencies)[i] == fileIndex)
        {
            return true;
        }
    }
    return false;
}

/**
 * @brief Determine if a given file name has already appears in the File's dependencies array.
 *        If the file exists, the function returns it's index in the array, otherwise the function
 *        returns -1
 * @param fileName The file name to check.
 * @param dependencies The array which contains the existing Files.
 * @return The index of the given file name in the array if exists, otherwise returns -1.
 */
static int containsFile(char const *fileName, File * const dependencies)
{
    assert(fileName != NULL);
    assert(dependencies != NULL);

    int i;
    for (i = 0 ; i < gNumberOfFiles; i++)
    {
        assert(dependencies[i].fileName != NULL);
        if (strcmp(dependencies[i].fileName, fileName) == STRING_EQUALITY)
        {
            return i;
        }
    }
    return FILE_NOT_FOUND;
}
