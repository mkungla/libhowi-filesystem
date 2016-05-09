<?php
/********************************************************************
 * Created by:	Marko Kungla @ OkramLabs on Aug 6, 2012 - 9:21:34
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
 * File:		Filesystem.inc
 * @link		https://
 ********************************************************************
 * Contributors:
 * @author		Marko Kungla <okramlabs@gmail.com>
 *  	Github:	https://github.com/mkungla
 ********************************************************************
 * Comments:
 */
namespace HOWI3\libhowi\Filesystem\php7;

use \HOWI3\libhowi\Filesystem\Commons\AbstractFilesystem;
use \HOWI3\libhowi\Filesystem\Commons\FilesystemInterface;
use \HOWI3\libhowi\Filesystem\Commons\SharedMethodsInterface;
use \HOWI3\libhowi\Filesystem\Commons\TraitForResponse;
use \HOWI3\libhowi\Filesystem\php7\Objects\DirectoryPlaceholderObject;
use \HOWI3\libhowi\Filesystem\php7\Objects\DirectoryTreeObject;
use \HOWI3\libhowi\Filesystem\php7\Objects\FileObject;
use \HOWI3\libhowi\Filesystem\php7\Objects\InfoObject;
use \HOWI3\libhowi\Filesystem\php7\Objects\TmpObject;
use \HOWI3\libhowi\Filesystem\php7\Objects\LinkObject;
use \HOWI3\libhowi\Filesystem\php7\TraitForFileSystem;
use \HOWI3\libhowi\Filesystem\php7\TraitForSharedMethods;

class Filesystem extends AbstractFilesystem implements FilesystemInterface, SharedMethodsInterface
{
    use TraitForResponse;
    use TraitForFileSystem;
    use TraitForSharedMethods;

    /**
     *
     * {@inheritDoc}
     *
     */
    public function __construct($setCwd = false)
    {
        $this->debug(801);
        $this->setStatus(true);
        if (! $this->setCwd($setCwd)) {
            $append = error_get_last();
            $this->warning(500, $append['message']);
        }
        
        $this->tmp()->setTmp();
    }

    /**
     *
     * {@inheritDoc}
     *
     */
    public function dir($directory = false, $dirname = false, $recursive = true, $mode = false, $context = false)
    {
        if (empty($directory)) {
            return false;
        }
        // ///////////
        $this->debug(807);
        if (array_key_exists($directory, $this->dirkeys) &&
             array_key_exists($this->dirkeys[$directory], $this->dirs) &&
             is_object($this->dirs[$this->dirkeys[$directory]])) {
            $response = $this->dirs[$this->dirkeys[$directory]];
            $this->response = $this->dirs[$this->dirkeys[$directory]]->response();
        } elseif (! empty($directory) && ! empty($dirname)) {
            
            $dir = $this->makeAbsolute($dirname . DIRECTORY_SEPARATOR . $directory);
            $HID = md5($dir);
            $this->dirkeys[$directory] = $HID;
            $this->dirs[$HID] = $this->isDir($dir) ? new DirectoryTreeObject($dir, 
                DirectoryTreeObject::SKIP_DOTS) : new DirectoryPlaceholderObject($dir, $recursive, $mode, 
                $context, $this->getLogFile(), $this->getLogLevel(), $this->getUID(), $this->getUsername());
            
            if ($this->dirs[$HID] instanceof DirectoryPlaceholderObject) {
                $this->setStatus($this->dirs[$HID]->getStatus());
                $this->setCode($this->dirs[$HID]->getCode());
            }
            
            if ($this->dirs[$HID] instanceof DirectoryTreeObject) {
                $this->dirs[$HID]->setFileClass('\HOWI3\libhowi\Filesystem\php7\Objects\FileObject');
                $this->dirs[$HID]->setInfoClass('\HOWI3\libhowi\Filesystem\php7\Objects\InfoObject');
                $this->dirs[$HID]->setLogFile($this->getLogFile());
                $this->dirs[$HID]->setLogLevel($this->getLogLevel());
                $this->dirs[$HID]->setUID($this->getUID());
                $this->dirs[$HID]->setUsername($this->getUsername());
                
                $this->response->setStatus(true);
            } else {
                
                /* We don't need DirectoryPlaceholderObject anymore for this directory */
                if ($this->isDir($dir)) {
                    
                    $this->dirs[$HID] = new DirectoryTreeObject($dir, DirectoryTreeObject::SKIP_DOTS);
                    $this->dirs[$HID]->setFileClass('\HOWI3\libhowi\Filesystem\php7\Objects\FileObject');
                    $this->dirs[$HID]->setInfoClass('\HOWI3\libhowi\Filesystem\php7\Objects\InfoObject');
                    $this->dirs[$HID]->setLogFile($this->getLogFile());
                    $this->dirs[$HID]->setLogLevel($this->getLogLevel());
                    $this->dirs[$HID]->setUID($this->getUID());
                    $this->dirs[$HID]->setUsername($this->getUsername());
                }
            }
            
            $response = $this->dirs[$HID];
        }
        return $response;
    }

    /**
     *
     * {@inheritDoc}
     *
     */
    public function file($filename = false, $dirname = false, $data = '', $flags = FILE_APPEND, $context = null)
    {
        if (! empty($filename) && array_key_exists($filename, $this->files) &&
             is_object($this->files[$filename]))
            return $this->files[$filename];
        
        $this->debug(808);
        if (empty($filename)) {
            $this->notice(601);
            return false;
        }
        
        $dirname = empty($dirname) ? $this->getCwd() : $this->makeAbsolute($dirname);
        $real = $dirname . DIRECTORY_SEPARATOR . $filename;
        $real = str_replace(DIRECTORY_SEPARATOR . DIRECTORY_SEPARATOR, DIRECTORY_SEPARATOR, $real);
        
        if (! $this->exists($real) && ! $this->isWritable(dirname($real))) {
            $this->warning(503, $real);
            return false;
        } elseif (! $this->exists($real) && $this->isWritable(dirname($real))) {
            $result = empty($context) || ! is_resource($context) ? file_put_contents($real, $data, $flags) : file_put_contents(
                $real, $data, $flags, $context);
            $result = $result !== false ? $this->info(703, $real) : $this->warning(504, $real);
        }
        
        if ($this->exists($real)) {
            $this->files[$filename] = new FileObject($real, 'r+');
            $this->files[$filename]->setFileClass('\HOWI3\libhowi\Filesystem\php7\Objects\FileObject');
            $this->files[$filename]->setInfoClass("\HOWI3\libhowi\Filesystem\php7\Objects\InfoObject");
            $result = $this->files[$filename];
        }
        
        return $result;
    }

    /**
     *
     * {@inheritDoc}
     *
     */
    public function infoObject($basename = false, $directory = false)
    {
        if (! empty($basename) && array_key_exists($basename, $this->infos) && is_object(
            $this->infos[$basename]))
            return $this->infos[$basename];
        
        $this->debug(809);
        if (empty($basename)) {
            $this->notice(602);
            return false;
        }
        
        $dirname = empty($directory) ? $this->getCwd() : $this->makeAbsolute($directory);
        $real = $dirname . DIRECTORY_SEPARATOR . $basename;
        $real = str_replace(DIRECTORY_SEPARATOR . DIRECTORY_SEPARATOR, DIRECTORY_SEPARATOR, $real);
        
        
        if (! $this->exists($real) && ! $this->isReadable($real)) {
            $this->warning(504, $real);
            $result = false;
        } elseif ($this->exists($real)) {
            $this->infos[$basename] = new InfoObject($real);
            $this->infos[$basename]->setFileClass('\HOWI3\libhowi\Filesystem\php7\Objects\FileObject');
            $this->infos[$basename]->setInfoClass("\HOWI3\libhowi\Filesystem\php7\Objects\InfoObject");
            $result = $this->infos[$basename];
        }
        return $result;
    }

    /**
     *
     * {@inheritDoc}
     *
     */
    public function tmp($keyword = 'tmp')
    {
        if (array_key_exists($keyword, $this->tmp) && is_object($this->tmp[$keyword]) &&
             $this->tmp[$keyword] instanceof \HOWI3\libhowi\Filesystem\Commons\ObjectInterfaces\TmpInterface) {
            return $this->tmp[$keyword];
        } else {
            $this->tmp[$keyword] = new TmpObject();
            return $this->tmp[$keyword];
        }
    }

    /**
     *
     * {@inheritDoc}
     *
     */
    public function link($keyword = 'link')
    {
        if (array_key_exists($keyword, $this->link) && is_object($this->link[$keyword]) &&
             $this->link[$keyword] instanceof \HOWI3\libhowi\Filesystem\Commons\ObjectInterfaces\LinkInterface) {
            return $this->link[$keyword];
        } else {
            $this->link[$keyword] = new LinkObject();
            return $this->link[$keyword];
        }
    }
}
