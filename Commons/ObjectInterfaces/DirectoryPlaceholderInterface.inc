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
 * File:		DirectoryPlaceholderInterface.inc
 * @link		https://github.com/okramlabs/libhowi-filesystem
 ********************************************************************
 * Contributors:
 * @author		Marko Kungla <okramlabs@gmail.com>
 *  	Github:	https://github.com/mkungla
 ********************************************************************
 * Comments:
 */
namespace HOWI3\libhowi\Filesystem\Commons\ObjectInterfaces;

interface DirectoryPlaceholderInterface
{

    /**
     * Make directory a new directory if does not exist
     *
     * This function calls $this->dirCreate() function
     *
     * @param string $pathname            
     * @param string $mode            
     * @param string $recursive            
     * @param string $context            
     * @return bool
     */
    public function __construct($pathname = false, $mode = false, $recursive = true, $context = false);

    /**
     * Set path for directory object
     *
     * This can be relative path
     *
     * @param string $pathname            
     * @param string $HID            
     * @return bool
     */
    public function dirsetPath($pathname = false, $HID = false);

    /**
     * Set Directory mode
     *
     * Mode is non octal like 755 function will convert int nto correct octal value
     *
     * @param string $mode            
     * @param string $HID            
     * @return bool
     */
    public function dirsetMode($mode = false, $HID = false);

    /**
     * Set Recursive on off while creating directory
     *
     * @param string $recursive            
     * @param string $HID            
     * @return bool
     */
    public function dirsetRecursive($recursive = false, $HID = false);

    /**
     * Set directory context
     *
     * @param string $context            
     * @param string $HID            
     * @return bool
     */
    public function dirsetContext($context = false, $HID = false);

    /**
     * Create this directory from current object
     *
     * @param string $HID            
     * @return bool
     */
    public function dirCreate($HID = false);
}
