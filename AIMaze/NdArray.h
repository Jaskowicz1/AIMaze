#pragma once
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

class NdArray
{
public:
    
    NdArray(const int rowAmount, const int colAmount)
    {
        // Set array size.
        array.resize(rowAmount);
        for(auto& column : array)
        {
            column.resize(colAmount);
            // Fill row with 0.
            std::fill(column.begin(), column.end(), 0);
        }
    }

    /**
     * \brief Load a file with the array's data stored inside.
     * \param fileName The file name.
     * \return "true" if the file was loaded successfully.
     */
    inline bool load(const std::string& fileName)
    {
        std::ifstream file(fileName);

        if(!file.good())
        {
            std::cout << fileName << " Does not exist." << "\n";
            return false;
        }
		
        std::string line;

        int row = 0;
        int col = 0;

        std::cout << "Loading Q-Table..." << "\n";

        // Read each line in the file.
        while (std::getline(file, line))
        {
            std::cout << "col: " << col << "\n";
            
            array[row][col] = std::stof(line);

            //std::cout << "col: " << col << " / " << (array[row].capacity() - 1) << "\n";

            // If the columns match the size of the board (-1 because we're counting from 0)
            // then increase the row and reset columns to 0.
            if(col / (array[row].capacity() - 1) == 1)
            {
                std::cout << "Reached capacity, increasing row." << "\n";
                row++;
                col = 0;
                continue;
            }

            col++;
        }

        // Close the file.
        file.close();

        return true;
    }

    /**
     * \brief Save the array to a file specified.
     * \param fileName The file to save the array to.
     * \return "true" if the array was successfully saved to the file.
     */
    inline bool save(const std::string& fileName)
    {
        std::ofstream file(fileName);
        if (!file.good())
        {
            std::cout << fileName << " Does not exist." << "\n";
            return false;
        }

        for(const auto& col : array)
        {
            int count = 0;
            for(const float val : col)
            {
                file << val;
                if(count++ != col.capacity() - 1)
                {
                    file << "\n";
                }
            }
        }
        file.close();

        return true;
    }

    /**
     * \brief Get the highest value in a row.
     * \param row The row to get the value from.
     * \return The highest value from a row.
     */
    inline float max(const int row)
    {
        return *std::max_element(array[row].begin(), array[row].end());
    }

    /**
     * \brief Get the index of the highest value from a row.
     * \param row The row to get the index from.
     * \return The index of the highest value from a row.
     */
    inline int argmax(const int row)
    {
        return std::distance(array[row].begin(), std::max_element(array[row].begin(), array[row].end()));
    }

    /**
     * \brief Output the array into a human-friendly format (similar to a Python array).
     * \return The array in a human-friendly format.
     */
    inline std::string ArrayToString()
    {
        std::string str = "[";

        int rowCount = 0;

        for(const auto& col : array)
        {
            str += "[";
            int colCount = 0;
            for(const float val : col)
            {
                str += std::to_string(val);
                if(colCount++ != col.capacity() - 1)
                {
                    str += ", ";
                    continue;
                }

                str += "]";
            }

            if(rowCount++ != array.capacity() - 1)
            {
                str += "\n";
                continue;
            }

            str += "]";
        }

        return str;
    }

    /**
     * \brief Get the array that stores all the data.
     * \return The array with all data.
     */
    inline std::vector<std::vector<float>> GetArray()
    {
        return array;
    }
    
protected:

    // The array with all data.
    std::vector<std::vector<float>> array;
    
};
