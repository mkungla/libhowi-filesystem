<?php
/*******************************************************************
 * Created by:  Marko Kungla @ OkramLabs on Aug 6, 2012 - 8:59:28
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
 * File:      DirectoryTreeObject.inc
 * @link      https://
 ********************************************************************
 * Contributors:
 * @author Marko Kungla <okramlabs@gmail.com>
 *           Github: https://github.com/mkungla
 ********************************************************************
 * Comments:
 */
namespace HOWI3\libhowi\Filesystem\php5\Objects;

use \RecursiveDirectoryIterator;
use \RecursiveIteratorIterator;
use \HOWI3\libhowi\Filesystem\Commons\ObjectInterfaces\DirectoryTreeInterface;
use \HOWI3\libhowi\Filesystem\Commons\TraitForResponse;
use \HOWI3\libhowi\Filesystem\php5\TraitForSharedMethods;

class DirectoryTreeObject extends RecursiveDirectoryIterator implements DirectoryTreeInterface
{
    use TraitForResponse;
    use TraitForSharedMethods;

    /**
     *
     * @var array $dirkeys Reqistered basenames
     */
    private $dirkeys = [];

    /**
     *
     * {@inheritDoc}
     *
     */
    public function c($basename = false)
    {
        if (! empty($basename) && array_key_exists($basename, $this->dirkeys))
            $c = $this->dirkeys[$basename];
        elseif (! empty($basename) && is_dir($this->getPath() . DIRECTORY_SEPARATOR . $basename)) {
            $c = new DirectoryTreeObject($this->getPath() . DIRECTORY_SEPARATOR . $basename, 
                DirectoryTreeObject::SKIP_DOTS);
            $c->setFileClass('\HOWI3\libhowi\Filesystem\php5\Objects\FileObject');
            $c->setInfoClass('\HOWI3\libhowi\Filesystem\php5\Objects\InfoObject');
            $c->setLogFile($this->getLogFile());
            $c->setLogLevel($this->getLogLevel());
            $c->setUID($this->getUID());
            $c->setUsername($this->getUsername());
        } 

        else
            $c = false;
        
        return $this->dirkeys[$basename] = $c;
    }

    /**
     *
     * {@inheritDoc}
     *
     */
    public function hasChildren($allow_links = false)
    {
        return $this->valid();
    }

    /**
     *
     * {@inheritDoc}
     *
     */
    public function ls($sort = false)
    {
        $this->rewind();
        
        $ls = [
            'dir' => [],
            'link' => [],
            'file' => []
        ];
        $display = [];
        
        while ($this->valid()) {
            if (! $this->isDot()) {
                if ($this->isDir() && ! $this->isLink() && ! empty($sort))
                    $ls['dir'][$this->getFilename()] = $this->getType();
                
                elseif ($this->isLink() && ! empty($sort))
                    $ls['link'][$this->getFilename()] = $this->getType();
                
                elseif ($this->isFile() && ! empty($sort))
                    $ls['file'][$this->getFilename()] = $this->getType();
                else
                    $display[$this->getFilename()] = $this->getType();
                
                if ($this->isDir() || $this->isLink()) {
                    $this->dirkeys[$this->getFilename()] = $this->getChildren();
                }
            }
            
            $this->next();
        }
        if (! empty($sort)) {
            
            ksort($ls['dir']);
            ksort($ls['link']);
            ksort($ls['file']);
            
            $display = array_merge($ls['dir'], $ls['link'], $ls['file']);
        }
        return $display;
    }

    /**
     *
     * {@inheritDoc}
     *
     */
    public function lsInfo($sort = false, $convert = false, $timeformat = false)
    {
        $this->rewind();
        
        $ls = [
            'dir' => [],
            'link' => [],
            'file' => []
        ];
        $display = [];
        $lsinfo = [];
        
        while ($this->valid()) {
            
            if (! $this->isDot()) {
                $cname = $this->getFilename();
                $lsinfo[$cname] = new \stdClass();
                $lsinfo[$cname]->name = $this->getFilename();
                $lsinfo[$cname]->type = $this->getType();
                $lsinfo[$cname]->size = $this->getSize($convert);
                
                $lsinfo[$cname]->taccess = $this->getATime($timeformat);
                $lsinfo[$cname]->tchange = $this->getCTime($timeformat);
                $lsinfo[$cname]->tmodify = $this->getMTime($timeformat);
                
                if ($this->isDir() && ! $this->isLink() && ! empty($sort))
                    $ls['dir'][$cname] = $lsinfo[$cname]->type;
                
                elseif ($this->isLink() && ! empty($sort))
                    $ls['link'][$cname] = $lsinfo[$cname]->type;
                
                elseif ($this->isFile() && ! empty($sort))
                    $ls['file'][$cname] = $lsinfo[$cname]->type;
                else
                    $display[$cname] = $lsinfo[$cname]->type;
            }
            $this->next();
        }
        if (! empty($sort)) {
            ksort($ls['dir']);
            ksort($ls['link']);
            ksort($ls['file']);
            
            $display = array_merge($ls['dir'], $ls['link'], $ls['file']);
        }
        foreach ($display as $row => $type) {
            $display[$row] = $lsinfo[$row];
        }
        return $display;
    }

    /**
     *
     * {@inheritDoc}
     *
     */
    public function lsTree()
    {
        $DirectoryTreeObject = new RecursiveDirectoryIterator($this->getPath(), 
            RecursiveDirectoryIterator::SKIP_DOTS);
        $DirectoryTreeObject->setInfoClass('\HOWI3\libhowi\Filesystem\php5\Objects\InfoObject');
        
        $ritit = new RecursiveIteratorIterator($DirectoryTreeObject, RecursiveIteratorIterator::CHILD_FIRST);
        $tree = array();
        foreach ($ritit as $splFileInfo) {
            $path = $splFileInfo->isDir() ? array(
                $splFileInfo->getFilename() => array()
            ) : array(
                $splFileInfo->getFilename()
            );
            
            for ($depth = $ritit->getDepth() - 1; $depth >= 0; $depth --) {
                $path = array(
                    $ritit->getSubIterator($depth)
                        ->current()
                        ->getFilename() => $path
                );
            }
            $tree = array_merge_recursive($tree, $path);
        }
        return $tree;
    }
}
