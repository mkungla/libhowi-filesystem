<?php
/********************************************************************
 * Created by:	Marko Kungla @ OkramLabs on Aug 6, 2012 - 9:32:04
 * Contact:		okramlabs@gmail.com - https://okramlabs.com
 * @copyright:	2015 OkramLabs - https://okramlabs.com
 * @license		MIT
 *
 * Package name:libhowi-filesystem
 * @category	HOWI3
 * @package		libhowi
 * @subpackage	filesystem
 * 
 * Lang:		PHP
 * Encoding:	UTF-8
 * File:		FileInterface.inc
 * @link		https://
 ********************************************************************
 * Contributors:
 * @author		Marko Kungla <okramlabs@gmail.com>
 *  	Github:	https://github.com/mkungla
 ********************************************************************
 * Comments:
 */
namespace HOWI3\libhowi\Filesystem\Commons\ObjectInterfaces;

interface FileInterface
{

    /**
     * current
     *
     * Retrieve current line of file
     *
     * @return string|array Retrieves the current line of the file.
     *         If the FileObject::READ_CSV flag is set, this method returns
     *         an array containing the current line parsed as CSV data.
     */
    public function current();

    /**
     * Reached end of file
     *
     * Determine whether the end of file has been reached
     *
     * @return bool Returns TRUE if file is at EOF, FALSE otherwise.
     */
    public function eof();

    /**
     * Assert that if some wierd reason this is called in file
     * that we return false
     */
    public function getLinkTarget();

    /**
     * Portable file locking
     *
     * Locks or unlocks the file in the same portable way as flock().
     *
     * @param unknown $operation
     *            operation is one of the following:
     *            LOCK_SH to acquire a shared lock (reader).
     *            LOCK_EX to acquire an exclusive lock (writer).
     *            LOCK_UN to release a lock (shared or exclusive).
     *            LOCK_NB to not block while locking (not supported on Windows).
     *            
     * @param unknown $wouldblock
     *            Set to TRUE if the lock would block (EWOULDBLOCK errno condition).
     *            
     * @return bool Returns TRUE on success or FALSE on failure.
     */
    public function flock($operation, &$wouldblock = null);

    /**
     * FileObject does not have children
     *
     * @return bool Returns FALSE
     */
    public function hasChildren();

    /**
     * No purpose
     * inherited from iterator but useless
     * An FileObject does not have children so this method returns allways NULL.
     */
    public function getChildren();

    /**
     * Flushes the output to the file
     *
     * Forces a write of all buffered output to the file.
     *
     * @return Returns TRUE on success or FALSE on failure
     */
    public function fflush();

    /**
     * Gets character from file
     *
     * @return string|bool Returns a string containing a single character read
     *         from the file or FALSE on EOF.
     */
    public function fgetc();

    /**
     * Gets line from file
     *
     * @return string Returns a string containing the next line from the file,
     *         or FALSE on error.
     */
    public function fgets();

    /**
     * Gets line from file and strip HTML tags
     *
     * @param string $allowable_tags
     *            Optional parameter to specify tags which should not be stripped.
     *            
     * @return string Returns a string containing the next line of the file
     *         with HTML and PHP code stripped, or FALSE on error.
     */
    public function fgetss($allowable_tags = false);

    /**
     * Gets line from file and parse as CSV fields
     *
     * Gets a line from the file which is in CSV format and returns
     * an array containing the fields read.
     *
     * @param string $delimiter
     *            The field delimiter (one character only).
     *            Defaults as a comma or the value set using FileObject::setCsvControl().
     * @param string $enclosure
     *            The field enclosure character (one character only).
     *            Defaults as a double quotation mark or the value set using FileObject::setCsvControl().
     * @param string $escape
     *            The escape character (one character only).
     *            Defaults as a backslash (\) or the value set using FileObject::setCsvControl().
     *            
     * @return array Returns an indexed array containing the fields read, or FALSE on error.
     */
    public function fgetcsv($delimiter = ",", $enclosure = "\"", $escape = "\\");

    /**
     * Write a field array as a CSV line
     *
     * @param array $fields
     *            An array of values.
     * @param string $delimiter
     *            The optional delimiter parameter sets the field delimiter (one character only).
     * @param string $enclosure
     *            The optional enclosure parameter sets the field enclosure (one character only).
     * @param string $escape
     *            The optional escape parameter sets the escape character (one character only).
     *            
     * @return int|bool Returns the length of the written string or FALSE on failure.
     *         returns FALSE, and does not write the CSV line to the file, if the delimiter
     *         or enclosure parameter is not a single character.
     *        
     *         $fields = [], $delimiter = ",", $enclosure = '"', $escape = "\\"
     */
    public function fputcsv($fields);

    /**
     * Sets the delimiter and enclosure character for parsing CSV fields.
     *
     * @param string $delimiter
     *            The field delimiter (one character only).
     * @param string $enclosure
     *            The field enclosure character (one character only).
     * @param string $escape
     *            The field escape character (one character only).
     * @return void
     */
    public function setCsvControl($delimiter = ",", $enclosure = "\"", $escape = "\\");

    /**
     * Output all remaining data on a file pointer
     *
     * Reads to EOF on the given file pointer from the current position and
     * writes the results to the output buffer. You may need to call FileObject::rewind()
     * to reset the file pointer to the beginning of the file if you have already written data to the file.
     *
     * @return int Returns the number of characters read from handle and passed through to the output.
     */
    public function fpassthru();

    /**
     * Reads the given number or all of bytes from the file.
     *
     * @param string $length
     *            The number of bytes to read.
     * @return string|bool Returns the string read from the file or FALSE on failure.
     */
    public function fread($length);

    /**
     * Parses input from file according to a format
     *
     * Reads a line from the file and interprets it according to the specified format,
     * which is described in the documentation for sprintf(). Any whitespace in the format
     * string matches any whitespace in the line from the file. This means that even
     * a tab \t in the format string can match a single space character in the input stream.
     *
     * @param unknown $format
     *            The specified format as described in the sprintf() documentation.
     * @return array|int If only one parameter is passed to this method,
     *         the values parsed will be returned as an array. Otherwise, if optional parameters are passed,
     *         the function will return the number of assigned values. The optional parameters must be passed by reference.
     *        
     */
    public function fscanf($format);

    /**
     * Seek to a position
     *
     * Seek to a position in the file measured in bytes from the beginning of the file,
     * obtained by adding offset to the position specified by whence.
     *
     * @param unknown $offset
     *            The offset. A negative value can be used to move backwards through the
     *            file which is useful when SEEK_END is used as the whence value.
     * @param string $whence
     *            whence values are:
     *            
     *            SEEK_SET - Set position equal to offset bytes.
     *            SEEK_CUR - Set position to current location plus offset.
     *            SEEK_END - Set position to end-of-file plus offset.
     *            
     *            If whence is not specified, it is assumed to be SEEK_SET.
     *            
     * @return int Returns 0 if the seek was successful, -1 otherwise.
     *         Note that seeking past EOF is not considered an error.
     */
    public function fseek($offset, $whence = SEEK_SET);

    /**
     * Gets information about the file
     * Gathers the statistics of the file.
     * Behaves identically to fstat().
     *
     * @return array Returns an array with the statistics of the file;
     *         the format of the array is described in detail on the stat() manual page.
     */
    public function fstat();

    /**
     * Return current file position
     *
     * Returns the position of the file pointer which represents the current offset in the file stream.
     *
     * @return int Returns the position of the file pointer as an integer, or FALSE on error.
     */
    public function ftell();

    /**
     * Truncates the file to a given lengthm to size bytes.
     *
     *
     * @param int $size
     *            If size is larger than the file it is extended with null bytes.
     *            If size is smaller than the file, the extra data will be lost.
     *            
     * @return bool Returns TRUE on success or FALSE on failure.
     */
    public function ftruncate($size);

    /**
     * Writes the contents of string to the file
     *
     * @param string $str
     *            The string to be written to the file.
     * @param int $length
     *            If the length argument is given, writing will stop after
     *            length bytes have been written or the end of string is reached, whichever comes first.
     *            
     * @return int Returns the number of bytes written, or NULL on error.
     */
    public function fwrite($str, $length = false);

    /**
     * Get the delimiter and enclosure character for CSV
     *
     * @return array Returns an indexed array containing the delimiter and enclosure character.
     */
    public function getCsvControl();

    /**
     * Alias of FileObject::fgets()
     */
    public function getCurrentLine();

    /**
     * Gets flags for the FileObject
     * gets the flags set for an instance of FileObject as an integer.
     *
     * @return int Returns an integer representing the flags.
     */
    public function getFlags();

    /**
     * Get maximum line length
     * Gets the maximum line length as set by FileObject::setMaxLineLen().
     *
     *
     * @return int Returns the maximum line length if one has been set with FileObject::setMaxLineLen(), default is 0.
     */
    public function getMaxLineLen();

    /**
     * Get line number
     * This number may not reflect the actual line number in the file if
     * FileObject::setMaxLineLen() is used to read fixed lengths of the file.
     *
     *
     * @return Returns the current line number.
     */
    public function key();

    /**
     * Moves ahead to the next line in the file.
     *
     * @return void
     */
    public function next();

    /**
     * Rewinds the file back to the first line.
     *
     * @return void
     */
    public function rewind();

    /**
     * Seek to specified line
     *
     * @param int $line_pos
     *            The zero-based line number to seek to.
     * @return void
     */
    public function seek($line_pos);

    /**
     * Sets flags for the FileObject
     *
     * @param int $flags
     *            Bit mask of the flags to set. See FileObject constants for the available flags.
     * @return void
     */
    public function setFlags($flags);

    /**
     * Sets the maximum length of a line to be read.
     *
     * @param int $max_len
     *            The maximum length of a line.
     * @return void
     *
     */
    public function setMaxLineLen($max_len);

    /**
     * Alias of FileObject::current()
     */
    public function __toString();

    /**
     * Check whether EOF has been reached.
     *
     * @return bool Returns TRUE if not reached EOF, FALSE otherwise.
     */
    public function valid();
}
