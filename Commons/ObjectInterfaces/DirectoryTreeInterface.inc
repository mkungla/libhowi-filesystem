<?php
/*******************************************************************
 * Created by:  Marko Kungla @ OkramLabs on Aug 6, 2012 - 9:00:29
 * Contact:     okramlabs@gmail.com - https://okramlabs.com
 * @copyright   2015 OkramLabs - https://okramlabs.com
 * @license     MIT
 *
 * Package name:libhowi-filesystem
 * @category	HOWI3
 * @package		libhowi
 * @subpackage	filesystem
 * 
 * Lang:        PHP
 * Encoding:    UTF-8
 * File:        DirectoryTreeInterface.inc
 * @link        https://
 ********************************************************************
 * Contributors:
 * @author Marko Kungla <okramlabs@gmail.com>
 *           Github: https://github.com/mkungla
 ********************************************************************
 * Comments:
 */
namespace HOWI3\libhowi\Filesystem\Commons\ObjectInterfaces;

interface DirectoryTreeInterface
{

    /**
     * Returns an iterator for the requested child item if it is a directory
     *
     * @return DirectoryTreeObject
     */
    public function c($basename = false);

    /**
     * hasChildren override
     *
     * Original inherited RecursiveDirectoryIterator::hasChildren()
     * function returns whether current entry is a directory and not '.' or '..'
     * However this function tells wether current item is a directory and has items in the directory
     * NB The iterators pointer position do not affect this function
     *
     * @param string $allow_links            
     * @return boolean
     */
    public function hasChildren($allow_links = false);

    /**
     * Returns an iterator for the current entry if it is a directory
     *
     * @return DirectoryTreeObject
     */
    public function getChildren();

    /**
     * Get sub path
     *
     * @return string
     */
    public function getSubPath();

    /**
     * Get sub path and name
     *
     * @return string
     */
    public function getSubPathname();

    /**
     * Rewind the DirectoryIterator back to the start
     *
     * @return void
     */
    public function rewind();

    /**
     * Move forward to next DirectoryIterator item
     *
     * @return void
     */
    public function next();

    /**
     * Return the key for the current DirectoryIterator item
     *
     * @return string
     */
    public function key();

    /**
     * Get the handling flags
     *
     * @return int The integer value of the set flags.
     */
    public function getFlags();

    /**
     * Sets handling flags
     *
     * @param int $flags            
     * @return void
     */
    public function setFlags($flags);

    /**
     * Return file name of current DirectoryIterator item.
     *
     * @return string
     */
    public function getFilename();

    /**
     * Gets the file extension
     *
     * @return string
     */
    public function getExtension();

    /**
     * Determine if current DirectoryIterator item is '.' or '..'
     *
     * @return boolean
     */
    public function isDot();

    /**
     * Check whether current DirectoryIterator position is a valid file
     *
     * @return boolean
     */
    public function valid();

    /**
     * Seek to a DirectoryIterator item
     * 
     * @return void
     */
    public function seek($position);

    /**
     * Get file name as a string
     * 
     * @return string
     */
    public function __toString();
    
    /**
     * Return path and file name of current DirectoryIterator item
     *
     * @return string $pathnamae
     */
    public function getPathname();
    
    /**
     * Get the permissions of current DirectoryIterator item (non octal)
     *
     * @return int like 755
     */
    public function getPerms();
    
    /**
     * Get inode for the current DirectoryIterator item
     *
     * @return int inode
     */
    public function getInode();
    
    /**
     * Get owner ID of current DirectoryIterator item
     *
     * @return int ID
     */
    public function getOwner();
    
    /**
     * Get group ID for the current DirectoryIterator item
     *
     * @return int group id
     */
    public function getGroup();
    
    /**
     * list directory contents by key => value
     * @param string $sort
     * @return array key value
     */
    public function ls($sort = false);
    
    /**
     * list directory contents by key => value (holding info object)
     * stdClass();
     * ->name = $this->getFilename();
     * ->type = $this->getType();
     * ->size = $this->getSize($convert);
     * ->taccess = $this->getATime($timeformat);
     * ->tchange = $this->getCTime($timeformat);
     * ->tmodify = $this->getMTime($timeformat);
     * @param string $sort
     * @return array of objects
     */
    public function lsInfo($sort = false, $convert = false, $timeformat = false);
    
    /**
     * Get a nested directory tree as a multidimensional array for curent item
     */
    public function lsTree();
}