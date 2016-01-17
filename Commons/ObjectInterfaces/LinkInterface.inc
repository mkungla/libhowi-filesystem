<?php
/********************************************************************
 * Created by:	Marko Kungla @ OkramLabs on Aug 6, 2012 - 9:43:24
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
 * File:		LinkInterface.inc
 * @link		https://
 ********************************************************************
 * Contributors:
 * @author		Marko Kungla <okramlabs@gmail.com>
 *  	Github:	https://github.com/mkungla
 ********************************************************************
 * Comments:
 */
namespace HOWI3\libhowi\Filesystem\Commons\ObjectInterfaces;

interface LinkInterface
{

    /**
     * symlink — Creates a symbolic link
     */
    public function symlink($target = false, $link = false);

    /**
     * lchgrp — Changes group ownership of symlink
     */
    public function lchgrp($link = false, $group = false);

    /**
     * lchown — Changes user ownership of symlink
     */
    public function lchown($link = false, $user = false);

    /**
     * link — Create a hard link
     */
    public function hardlink($target = false, $link = false);

    /**
     * linkinfo — Gets information about a link
     */
    public function linkinfo($link = false);

    /**
     * lstat — Gives information about a file or symbolic link
     */
    public function lstat($link = false);

    /**
     * array_diff(stat($link), lstat($link))
     *
     * Shows diferences between stat and lstat
     * 
     * @param string $link            
     * @return array
     */
    public function lstatDiff($link = false);

    /**
     * readlink — Returns the target of a symbolic link
     */
    public function readlink($link = false);
}
