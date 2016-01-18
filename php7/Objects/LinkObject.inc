<?php
/*******************************************************************
 * Created by:  Marko Kungla @ OkramLabs on Aug 6, 2012 - 9:31:36
 * Contact:     okramlabs@gmail.com - https://okramlabs.com
 * @copyright   2015 OkramLabs - https://okramlabs.com
 * @license     MIT
 *
 * Package name:libhowi-filesystem
 * @category	HOWI3
 * @package		libhowi
 * @subpackage	filesystem
 * 
 * Lang:      PHP
 * Encoding:  UTF-8
 * File:      LinkTrait.inc
 * @link      https://
 ********************************************************************
 * Contributors:
 * @author Marko Kungla <okramlabs@gmail.com>
 *           Github: https://github.com/mkungla
 ********************************************************************
 * Comments:
 */
namespace HOWI3\libhowi\Filesystem\php7\Objects;

use HOWI3\libhowi\Filesystem\php7\TraitForSharedMethods;
use HOWI3\libhowi\Filesystem\Commons\ObjectInterfaces\LinkInterface;

class LinkObject implements LinkInterface
{
    
    use TraitForSharedMethods;

    /**
     *
     * {@inheritDoc}
     *
     */
    public function symlink($target = false, $link = false)
    {
        if (empty($target) || empty($link))
            return false;
        
        $target = $this->makeAbsolute($target);
        $link = $this->makeAbsolute($link);
        
        if (! file_exists($target) || file_exists($link))
            return false;
        
        return symlink($target, $link);
    }

    /**
     *
     * {@inheritDoc}
     *
     */
    public function hardlink($target = false, $link = false)
    {
        if (empty($target) || empty($link))
            return false;
        
        $target = $this->makeAbsolute($target);
        $link = $this->makeAbsolute($link);
        
        if (! file_exists($target) || is_dir($target) || file_exists($link))
            return false;
        
        return link($target, $link);
    }

    /**
     *
     * {@inheritDoc}
     *
     */
    public function lstat($link = false)
    {
        if (empty($link))
            return false;
        $link = $this->makeAbsolute($link);
        return @lstat($link);
    }

    /**
     *
     * {@inheritDoc}
     *
     */
    public function lstatDiff($link = false)
    {
        if (empty($link))
            return false;
        $link = $this->makeAbsolute($link);
        return array_diff(stat($link), lstat($link));
    }

    /**
     *
     * {@inheritDoc}
     *
     */
    public function readlink($link = false)
    {
        if (empty($link))
            return false;
        
        $link = $this->makeAbsolute($link);
        return readlink($link);
    }

    /**
     *
     * {@inheritDoc}
     *
     */
    public function lchgrp($link = false, $group = false)
    {
        if (empty($link))
            return false;
        
        $link = $this->makeAbsolute($link);
        return lchgrp($link, $group);
    }

    /**
     *
     * {@inheritDoc}
     *
     */
    public function lchown($link = false, $group = false)
    {
        if (empty($link))
            return false;
        
        $link = $this->makeAbsolute($link);
        return lchown($link, $group);
    }

    /**
     *
     * {@inheritDoc}
     *
     */
    public function linkinfo($link = false)
    {
        if (empty($link))
            return false;
        
        $link = $this->makeAbsolute($link);
        return linkinfo($link);
    }
}
