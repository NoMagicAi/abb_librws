#pragma once

#include <abb_librws/v1_0/rws.h>

#include <string>


namespace abb :: rws
{
  /**
   * \brief IO signal as a resource.
   */
  struct IOSignalResource
  {
    /**
     * \brief A constructor.
     *
     * \param name name of the IO signal.
     */
    explicit IOSignalResource(std::string const& name)
    : name(name)
    {
    }

    /**
     * \brief IO signal name.
     */
    std::string name;
  };


  /**
   * \brief Controller operation mode subscription resource
   */
  struct OperationModeResource
  {
  };


  /**
   * \brief Controller state subscription resource
   */
  struct ControllerStateResource
  {
  };


  /**
   * \brief A class for representing a file resource.
   */
  struct FileResource
  {
    /**
     * \brief A constructor.
     *
     * \param filename specifying the name of the file.
     * \param directory specifying the directory of the file on the robot controller (set to $home by default).
     */
    FileResource(const std::string& filename,
                  const std::string& directory = v1_0::Identifiers::HOME_DIRECTORY)
    :
    filename(filename),
    directory(directory)
    {}

    /**
     * \brief The file's name.
     */
    std::string filename;

    /**
     * \brief The file's directory on the robot controller.
     */
    std::string directory;
  };
}