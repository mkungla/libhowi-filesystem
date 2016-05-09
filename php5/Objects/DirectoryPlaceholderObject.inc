<?php
/*******************************************************************
 * Created by:  Marko Kungla @ OkramLabs on Aug 6, 2012 - 10:02:48
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
 * File:      DirectoryPlaceholderObject.inc
 * @link      https://
 ********************************************************************
 * Contributors:
 * @author Marko Kungla <okramlabs@gmail.com>
 *           Github: https://github.com/mkungla
 ********************************************************************
 * Comments:
 */
namespace HOWI3\libhowi\Filesystem\php5\Objects;

use \Directory;
use \HOWI3\libhowi\Filesystem\Commons\ObjectInterfaces\DirectoryPlaceholderInterface;
use \HOWI3\libhowi\Filesystem\Commons\SharedMethodsInterface;
use \HOWI3\libhowi\Filesystem\Commons\TraitForResponse;
use \HOWI3\libhowi\Filesystem\php5\TraitForSharedMethods;

class DirectoryPlaceholderObject extends Directory implements DirectoryPlaceholderInterface, 
    SharedMethodsInterface
{
    use TraitForResponse;
    use TraitForSharedMethods;

    public $path = '';
    
    public $handle;
    
    private $mode = 750;

    private $recursive = true;

    private $context = false;

    /**
     *
     * {@inheritDoc}
     *
     */
    public function __construct($pathname = false, $recursive = true, $mode = false, $context = false, $logfile = false, 
        $loglevel = false, $UID = false, $username = false)
    {
        if (! empty($logfile) && file_exists($logfile) && ! empty($loglevel)) {
            $this->setLogfile($logfile);
            $this->setLoglevel($loglevel);
            $this->setUID($UID);
            $this->setUsername($username);
        }
        
        $pathname = (! empty($pathname) ? $this->makeAbsolute($pathname) : $this->path);
        
        $HID = md5($pathname);
        
        $this->dirsetPath($pathname, $HID);
        $this->dirsetMode(! empty($mode) ? $mode : $this->mode, $HID);
        
        $this->dirsetRecursive(! empty($recursive) ? true : false, $HID);
        
        $context = ! empty($context) ? $this->dirsetContext($context, $HID) : false;
        
        $this->info(701, $pathname);
        $this->response()->setStatus($this->dirCreate($HID));
    }

    /**
     *
     * {@inheritDoc}
     *
     */
    public function dirsetPath($pathname = false, $HID = false)
    {
        if (! empty($pathname) && is_string($pathname) && ! empty($HID)) {
            
            $this->path = $this->makeAbsolute($pathname);
            
            return true;
        } else {
            $this->debug(802, 'DirectoryTrait->dirsetPath');
            return false;
        }
    }

    /**
     *
     * {@inheritDoc}
     *
     */
    public function dirsetMode($mode = false, $HID = false)
    {
        if (! empty($HID) && ! empty($mode) && is_int($mode)) {
            $this->mode = $mode;
            return true;
        } else {
            
            $this->debug(802, 'DirectoryTrait->dirsetMode');
            return false;
        }
    }

    /**
     *
     * {@inheritDoc}
     *
     */
    public function dirsetRecursive($recursive = false, $HID = false)
    {
        if (! empty($HID) && is_bool($recursive)) {
            $this->recursive = $recursive;
            return true;
        } else {
            $this->debug(802, 'DirectoryTrait->dirsetRecursive');
        }
        
        return false;
    }

    /**
     *
     * {@inheritDoc}
     *
     */
    public function dirsetContext($context = false, $HID = false)
    {
        if (! empty($HID) && is_resource($context)) {
            $this->context = $context;
            return true;
        } 

        else {
            $this->debug(802, 'DirectoryTrait->dirsetContext');
            return false;
        }
    }

    /**
     *
     * {@inheritDoc}
     *
     */
    public function dirCreate($HID = false)
    {
        if ((! is_dir(dirname($this->path)) && $this->recursive === false) ||
             is_dir(dirname($this->path)) && ! $this->isWritable(dirname($this->path))) {
            $this->warning(502, $this->path);
            $result = false;
        } else {
            if (! empty($HID) && ! $this->exists($this->path) && ! empty($this->context)) {
                $result = mkdir($this->path, octdec(str_pad($this->mode, 4, 0, STR_PAD_LEFT)), 
                    $this->recursive, $this->context);
            }
            if (! empty($HID) && ! $this->exists($this->path) && empty($this->context)) {
                
                $result = mkdir($this->path, octdec(str_pad($this->mode, 4, 0, STR_PAD_LEFT)), true);
            }
        }
        return $result;
    }
}
