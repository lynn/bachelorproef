#ifndef VIS_FILE_H_INCLUDED
#define VIS_FILE_H_INCLUDED
/**
 * @file
 * Header for the VisualizerFile class.
 */

#include <fstream>
#include <string>
#include <vector>

namespace stride {
namespace output {

/**
 * Produces a file with daily cases count.
 */
class VisualizerFile
{
public:
	/// Constructor: initialize.
	VisualizerFile(const std::string& file = "stride_vis");

	/// Destructor: close the file stream.
	~VisualizerFile();

	/// Print the given visualisation data.
	void Print(int x);

private:
	/// Generate file name and open the file stream.
	void Initialize(const std::string& file);

private:
	std::ofstream m_fstream; ///< The file stream.
};

} // end_of_namespace
} // end_of_namespace

#endif // end of include guard
