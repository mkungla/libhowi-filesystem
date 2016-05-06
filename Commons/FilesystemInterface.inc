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
 * File:		FilesystemInterface.inc
 * @link		https://
 ********************************************************************
 * Contributors:
 * @author		Marko Kungla <okramlabs@gmail.com>
 *  	Github:	https://github.com/mkungla
 ********************************************************************
 * Comments:
 */
namespace HOWI3\libhowi\Filesystem\Commons;

interface FilesystemInterface
{

    /**
     * Construct with or without current working directory
     *
     * @param string $setCwd            
     */
    public function __construct($setCwd = false);

    /**
     * Creates Directory object with basename
     *
     * @param string $directory            
     * @param string $dirname            
     * @param bool $recursive            
     * @param int $mode            
     * @param resource $context            
     *
     * @return object DirectoryPlaceholderObject | DirectoryTreeObject
     */
    public function dir($directory = false, $dirname = false, $recursive = true, $mode = false, $context = false);

    /**
     * Creates FileObject object with basename
     *
     * @param string $filename            
     * @param string $dirname            
     * @param mixed $data            
     * @param int $flags            
     * @param resource $context            
     */
    public function file($filename = false, $dirname = false, $data = '', $flags = 0, $context = null);

    /**
     * Creates InfoObject object
     *
     * @param string $basename            
     * @param string $directory            
     */
    public function infoObject($basename = false, $directory = false);

    /**
     * Creates TmpObject object with keyword
     *
     * @param string $keyword            
     */
    public function tmp($keyword = 'tmp');

    /**
     * Creates LinkObject object with keyword
     *
     * @param string $keyword            
     */
    public function link($keyword = 'link');

    /**
     * Change current working directory.
     *
     * Rreturn true if chnging working dir is success.
     * Returns false if directory does not exist or can not be read
     *
     * @param string $path            
     * @param string $validate_dir_name
     *            base name of the directory
     *            useful when navigating ../../ and you want to make sure
     *            you are on directore named "$validate_dir_name"
     *            
     * @return bool
     */
    public function cd($path = false, $validate_dir_name = false);

    /**
     * Delete remove directories or files
     *
     * When shred is true then files will be destroyed by shred command before remove
     *
     * @param unknown $pathname            
     * @param string $shred            
     */
    public function rm($pathname, $shred = false);

    /**
     * List files and directories inside the specified path
     *
     * @param string $path            
     * @return array
     */
    public function scandir($path = false);

    /**
     * Gives information about a file or directory
     *
     * @param string $filename            
     * @return array
     */
    public function stat($filename = false);

    /**
     * getDiskTotalSpace
     *
     * Given a string containing a directory, this function will return the total
     * number of bytes on the corresponding filesystem or disk partition.
     *
     * @param string $partition_location            
     * @param string $convert            
     * @return number of bytes or formated output if $convert is not false
     */
    public function getDiskTotalSpace($partition_location = false, $convert = false);

    /**
     * * Given a string containing a directory, this function will return the number
     * of bytes available on the corresponding filesystem or disk partition.
     *
     * @param string $partition_location            
     * @param string $convert            
     * @return number of bytes or formated output if $convert is not false
     */
    public function getDiskFreeSpace($partition_location = false, $convert = false);

    /**
     * Attempts to change the group of the file filename to group.
     *
     * @param string $filename            
     * @param string $group            
     * @return bool
     */
    public function chgrp($filename = false, $group = false);

    /**
     * Attempts to change the mode of the specified file to that given in mode.
     *
     * @param string $filename            
     * @param string $mode            
     * @return bool
     */
    public function chmod($filename = false, $mode = false);

    /**
     * Attempts to change the owner of the file filename to user user.
     * Only the superuser may change the owner of a file.
     *
     * @param string $filename            
     * @param string $user            
     */
    public function chown($filename = false, $user = false);

    /**
     * clearstatcache — Clears file status cache
     * When you use stat(), lstat(), or any of the other functions listed in the affected
     * functions list (below), PHP caches the information those functions return in order to
     * provide faster performance.
     * However, in certain cases, you may want to clear the cached information
     * for instance you write some file and whant to read any stats after you should run
     * $FS->clearstatcache(false,$filename);
     *
     * @return void
     */
    public function clearstatcache($clear_realpath_cache = false, $filename = false);

    /**
     * Copies file or directory with all the contents to new location
     * bit similar as 'cp -R'
     * Makes a copy of the file source to dest.
     *
     * @param string $source            
     * @param string $dest            
     * @param string $context            
     * @return bool
     */
    public function copy($source = false, $dest = false, $context = false);

    /**
     * Renames (move) a file or entire directory
     *
     * @param string $oldname            
     * @param string $newname            
     * @param string $context            
     * @return bool
     */
    public function mv($oldname = false, $newname = false, $context = false);

    /**
     * Match filename against a pattern
     * FNM_NOESCAPE Disable backslash escaping.
     * FNM_PATHNAME Slash in string only matches slash in the given pattern.
     * FNM_PERIOD Leading period in string must be exactly matched by period in the given pattern.
     * FNM_CASEFOLD Caseless match. Part of the GNU extension.
     *
     * @param string $pattern            
     * @param string $string            
     * @param string $flags            
     *
     * @return bool
     */
    public function namePatternMatch($pattern = false, $string = false, $flags = false);

    /**
     * Find pathnames matching a pattern
     *
     * The getGlob() function searches for all the pathnames matching pattern
     * according to the rules used by the libc glob() function,
     * which is similar to the rules used by common shells.
     *
     * @param string $pattern            
     * @param number $flags            
     * @return array matching paths
     */
    public function getGlob($pattern = false, $flags = 0);

    /**
     * Tells whether the file was uploaded via HTTP POST
     *
     * @param string $filename            
     * @return bool
     */
    public function isUploadedFile($filename = false);

    /**
     * Get realpath cache entries
     *
     * @return array
     */
    public function getRealpathCache();

    /**
     * Get realpath cache size
     *
     * @return int
     */
    public function getRealpathCacheSize();

    /**
     * Closes DirectoryTreeObject or FileObjects for given basename
     *
     * While FileObjects are closed anyway automatically when FS gets destructed
     * but you might close the file your self. For inctance when multithreading / tasking
     *  
     * @param string $keyword
     * @return bool true|false
     */
    public function close($keyword = false);

    /**
     * Sets access and modification time of the item
     *
     * @param string $filename            
     * @param string $time            
     * @param string $atime            
     * @return bool
     */
    public function touch($filename = false, $time = false, $atime = false);

    /**
     * Returns all loaded directory keys
     *
     * @return array
     */
    public function getDirKeys();
    
    /**
     * Returns all loaded file keys
     *
     * @return array
     */
    public function getFileKeys();
    
    /**
     * Create a directory and file structure from custom array or
     * Make structure from DirectoryObject::lsTree output
     * If there happens to be collision between array so dir or file already exists
     * then it will be simply skipped. Directory must be a key and always habe array as value
     * even when directory is empty. Files have to have key regular int and value "string" 
     * filename 
     * 
     * @param string $rootpath atleast rootpath parent directory must exist! 
     * @param array $data_array 
     * @return bool true on success fase on failure
     */
    public function createStructure($rootpath,$data_array);
}
