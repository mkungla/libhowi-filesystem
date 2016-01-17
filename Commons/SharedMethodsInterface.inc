<?php
/********************************************************************
 * Created by:	Marko Kungla @ OkramLabs on Aug 6, 2012 - 9:58:05
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
 ********************************************************************
 * Contributors:
 * @author Marko Kungla <okramlabs@gmail.com>
 *           Github: https://github.com/mkungla
 ********************************************************************
 * Comments:
 */
namespace HOWI3\libhowi\Filesystem\Commons;

interface SharedMethodsInterface
{

    /**
     * Gets the current working directory
     *
     * @return string;
     */
    public function getCwd();

    /**
     * Gets file or directory size
     *
     * For directory it will return directory total size
     *
     * @param string $filename            
     * @param string $convert            
     * @return in file / directory size or formated array
     */
    public function getSize($convert = false, $filename = false);

    /**
     * Gets last access time of file
     *
     * @param string $timeformat            
     * @param string $filename            
     * @return int or formated output
     */
    public function getATime($timeformat = false, $pathname = false);

    /**
     * Gets inode change time of file
     *
     * @param string $timeformat            
     * @param string $filename            
     * @return int or formated output
     */
    public function getCTime($timeformat = false, $pathname = false);

    /**
     * Gets file modification time
     *
     * @param string $timeformat            
     * @param string $filename            
     * @return int or formated output
     */
    public function getMTime($timeformat = false, $pathname = false);

    /**
     * Conver to ago
     *
     * @param time $timestamp            
     * @param string $date_format
     *            a | y,m,d,h,i,s
     * @param bool $sfx
     *            short or long representation
     * @param string $ommit_zero
     *            ommit emty values?
     * @param string $ago_single            
     * @param array $clocale            
     */
    public function ct_ago($timestamp, $date_format = 'y,m,d,h,i,s', $sfx = true, $ommit_zero = true, $ago_single = true, $clocale = []);

    /**
     * Creates an FileObject object of the file.
     *
     * This is useful because FileObject contains additional methods
     * for manipulating the file whereas FileInfo is only useful
     * for gaining information, like whether the file is writable.
     *
     * @param string $open_mode            
     * @param string $use_include_path            
     * @param unknown $context            
     */
    public function openFile($open_mode = "r", $use_include_path = false, $context = null);

    /**
     * Sets the class used with InfoObject::openFile()
     *
     * @param string $class_name            
     */
    public function setFileClass($class_name = "\HOWI3\libhowi\Filesystem\php5\Objects\FileObject");

    /**
     * Sets the class used with InfoObject::getFileInfo() and SplFileInfo::getPathInfo()
     *
     * @param string $class_name            
     */
    public function setInfoClass($class_name = "\HOWI3\libhowi\Filesystem\php5\Objects\InfoObject");

    /**
     * Gets file owner username
     *
     * @param string $filename            
     * @return string owners username
     */
    public function getOwnerName($filename = false);

    /**
     * Gets file group name
     *
     * @param string $filename            
     * @return string group name
     */
    public function getGroupName($filename = false);

    /**
     * Returns canonicalized absolute pathname
     *
     * @param string $filename            
     * @return string
     */
    public function getRealpath($filename = false);

    /**
     * Returns information about a file path
     *
     * If you supply $filename so you are not calling it on FileObject, InfoObject
     * or DirectoryTreeObject you can specify following flags
     * PATHINFO_DIRNAME, PATHINFO_BASENAME, PATHINFO_EXTENSION or PATHINFO_FILENAME.
     *
     * otherwise it will return FileInfo object fo this items path
     *
     * @param string $filename            
     * @param int $flags            
     * @return mixed
     */
    public function getPathInfo($filename = false, $flags = false, 
        $class_name = '\HOWI3\libhowi\Filesystem\php5\Objects\InfoObject');

    /**
     * Get path of current Iterator item without filename or
     * Gets the path without filename
     *
     * @return string $path
     */
    public function getPath();

    /**
     * Gets the path to the file
     *
     * @return string $path
     */
    public function getPathname();

    /**
     * Gets file group
     *
     * @param string $filename            
     * @return int group id
     */
    public function getGroup($filename = false);

    /**
     * Gets file owner ID
     *
     * @param string $filename            
     * @return int owners user id
     */
    public function getOwner($filename = false);

    /**
     * Gets item inode
     *
     * @param string $filename            
     * @return int inode
     */
    public function getInode($filename = false);

    /**
     * Returns trailing name component of path
     *
     * Getting the path to a file or directory from current iterator (non dot) position,
     * this function will return the trailing name component. When getBasename is called
     * on Dot the current directory basename is returned instead!
     *
     * @param string $suffix            
     * @param string $pathname            
     * @return string Name of given path
     */
    public function getBasename($suffix = false, $pathname = false);

    /**
     * Gets file permissions (non octal)
     *
     * @param string $filename            
     * @return int like 755
     */
    public function getPerms($filename = false);

    /**
     * Gets file type
     *
     * @param string $filename            
     * @return string filetype
     */
    public function getType($filename = false);

    /**
     * Determine if current DirectoryIterator item is '.' or '..'
     * 
     * @return bool
     */
    public function isDot();

    /**
     * whether the file path is an absolute path.
     *
     * @param string $path            
     * @return bool
     */
    public function isAbsolute($path = false);

    /**
     * whether the file path is an relative path.
     *
     * @param string $path            
     * @return bool
     */
    public function isRelative($path = false);

    /**
     * Is path or directory writable
     *
     * @param string $path            
     * @return bool
     */
    public function isWritable($path = false);

    /**
     * Tells whether a file exists and is readable
     *
     * @param string $filename            
     * @return bool
     */
    public function isReadable($filename = false);

    /**
     * Tells whether the filename is a directory
     *
     * @param string $filename            
     * @return bool
     */
    public function isDir($filename = false);

    /**
     * Tells whether the filename is executable
     *
     * @param string $filename            
     * @return bool
     */
    public function isExecutable($filename = false);

    /**
     * Tells whether the filename is a regular file
     *
     * @param string $filename            
     * @return bool
     */
    public function isFile($filename = false);

    /**
     * Tells whether the filename is a symbolic link
     *
     * @param string $filename            
     * @return bool
     */
    public function isLink($filename = false);

    /**
     * Convert any path to absolute
     *
     * @param string $path            
     * @return string absolute path
     */
    public function makeAbsolute($path = false);

    /**
     * Does file or directory exist
     *
     * @param string $pathname            
     * @return bool
     */
    public function exists($pathname = false);

    /**
     * Return file name from path or Gets the file name from object
     *
     * @return array
     */
    public function getFilename($filename = false);

    /**
     * Gets the file extension
     *
     * @return string
     */
    public function getExtension($filename = false);
}
