<?php
/*******************************************************************
 * Created by:  Marko Kungla @ OkramLabs on Aug 6, 2012 - 9:36:42
 * Contact:     okramlabs@gmail.com - https://okramlabs.com
 * @copyright   2015 OkramLabs - https://okramlabs.com
 * @license     MIT
 *
 * Package name: libhowi-filesystem
 * @category     HOWI3
 * @package      libhowi
 * @subpackage   filesystem
 * 
 * Lang:      PHP
 * Encoding:  UTF-8
 * File:      TraitForFileSystem.inc
 * @link      https://
 ********************************************************************
 * Contributors:
 * @author Marko Kungla <okramlabs@gmail.com>
 *           Github: https://github.com/mkungla
 ********************************************************************
 * Comments:
 */
namespace HOWI3\libhowi\Filesystem\php5;

use \RecursiveIteratorIterator;
use \HOWI3\libhowi\Filesystem\php5\Objects\DirectoryTreeObject;

trait TraitForFileSystem
{

    /**
     *
     * {@inheritDoc}
     *
     */
    public function cd($path = false, $validate_dir_name = false)
    {
        return $this->setCwd($path, $validate_dir_name);
    }

    /**
     * rmdir
     *
     * {@inheritDoc}
     *
     */
    public function rm($pathname, $shred = false)
    {
        $pathname = $this->makeAbsolute($pathname);
        $response = false;
        if (is_dir($pathname)) {
            
            $files = array_diff(scandir($pathname), 
                array(
                    '.',
                    '..'
                ));
            foreach ($files as $file) {
                
                (is_dir("$pathname/$file") ? $this->rm("$pathname/$file", $shred) : (! empty($shred) &&
                     ! $this->isLink($pathname . DIRECTORY_SEPARATOR . $file) ? exec(
                        "shred -fzu $pathname" . DIRECTORY_SEPARATOR . "$file") : $this->rm(
                        $pathname . DIRECTORY_SEPARATOR . $file, $shred)));
            }
            
            $response = rmdir($pathname);
            $this->info(702, $pathname);
            $this->response()->setStatus($response);
        } else {
            $this->info(702, $pathname);
            $this->response()->setStatus($response);
            (! empty($shred) && ! $this->isLink($pathname) ? exec("shred -fzu $pathname") : unlink($pathname));
        }
        return $response;
    }

    /**
     *
     * {@inheritDoc}
     *
     */
    public function scandir($path = false)
    {
        return ! empty($path) && is_dir($this->makeAbsolute($path)) ? scandir($this->makeAbsolute($path)) : false;
    }

    /**
     *
     * {@inheritDoc}
     *
     */
    public function stat($filename = false)
    {
        return ! empty($filename) && file_exists($this->makeAbsolute($filename)) ? stat(
            $this->makeAbsolute($filename)) : false;
    }

    /**
     *
     * {@inheritDoc}
     *
     */
    public function getDiskTotalSpace($partition_location = false, $convert = false)
    {
        if (empty($partition_location))
            $partition_location = $this->getCwd();
        
        if (empty($convert))
            return disk_total_space($partition_location);
        
        $bytestotal = 0;
        $suffixes = array(
            'B',
            'kB',
            'MB',
            'GB',
            'TB',
            'PB',
            'EB',
            'ZB',
            'YB'
        );
        $bytestotal = disk_total_space($partition_location);
        
        $base = log($bytestotal, 1024);
        return array(
            round(pow(1024, $base - floor($base)), 2),
            $suffixes[floor($base)]
        );
    }

    /**
     *
     * {@inheritDoc}
     *
     */
    public function getDiskFreeSpace($partition_location = false, $convert = false)
    {
        if (empty($partition_location))
            $partition_location = $this->getCwd();
        
        if (empty($convert))
            return disk_free_space($partition_location);
        
        $result = [
            0,
            'B'
        ];
        $bytestotal = 0;
        $suffixes = array(
            'B',
            'kB',
            'MB',
            'GB',
            'TB',
            'PB',
            'EB',
            'ZB',
            'YB'
        );
        $bytestotal = disk_free_space($partition_location);
        
        $base = log($bytestotal, 1024);
        
        if ($bytestotal > 0) {
            $result = array(
                round(pow(1024, $base - floor($base)), 2),
                $suffixes[floor($base)]
            );
        }
        return $result;
    }

    /**
     *
     * {@inheritDoc}
     *
     */
    public function chgrp($filename = false, $group = false)
    {
        return ! empty($filename) && $this->exists($this->makeAbsolute($filename)) ? chgrp(
            $this->makeAbsolute($filename), $group) : false;
    }

    /**
     *
     * {@inheritDoc}
     *
     */
    public function chmod($filename = false, $mode = false)
    {
        $filename = $this->makeAbsolute($filename);
        if (file_exists($filename) && ! empty($mode) && is_int($mode))
            return chmod($filename, octdec(str_pad($mode, 4, 0, STR_PAD_LEFT)));
        else
            return false;
    }

    /**
     *
     * {@inheritDoc}
     *
     */
    public function chown($filename = false, $user = false)
    {
        $filename = $this->makeAbsolute($filename);
        if (file_exists($filename) && ! empty($user))
            return chown($filename, $user);
        else
            return false;
    }

    /**
     *
     * {@inheritDoc}
     *
     */
    public function clearstatcache($clear_realpath_cache = false, $filename = false)
    {
        if (! empty($filename) && ! empty($clear_realpath_cache))
            clearstatcache($clear_realpath_cache, $this->makeAbsolute($filename));
        else
            clearstatcache();
    }

    /**
     *
     * {@inheritDoc}
     *
     */
    public function copy($source = false, $dest = false, $context = false)
    {
        $source = $this->makeAbsolute($source);
        $dest = $this->makeAbsolute($dest);
        
        if (! empty($source) && ! empty($dest) && ! is_dir($source)) {
            
            if (empty($context))
                return copy($source, $dest);
            else
                return copy($source, $dest, $context);
        } elseif ((! empty($source) && ! empty($dest)) && is_dir($source) && ! file_exists($dest)) {
            
            $tmp_key = basename($source);
            $tmp_key2 = basename($dest);
            $tmp_src_key = $this->dir($tmp_key, dirname($source));
            
            $this->dir($tmp_key2, dirname($dest), false, $tmp_src_key->getPerms());
            $this->close($tmp_key);
            $this->close($tmp_key2);
            
            foreach ($iterator = new RecursiveIteratorIterator(
                new \RecursiveDirectoryIterator($source, \RecursiveDirectoryIterator::SKIP_DOTS),  // DirectoryTreeObject::CURRENT_AS_FILEINFO
                \RecursiveIteratorIterator::SELF_FIRST) as $item) {
                if ($item->isDir()) {
                    if (empty($context)) {
                        mkdir($dest . DIRECTORY_SEPARATOR . $iterator->getSubPathName(), 
                            octdec(str_pad($iterator->getPerms(), 4, 0, STR_PAD_LEFT)));
                    } 

                    else
                        mkdir($dest . DIRECTORY_SEPARATOR . $iterator->getSubPathName(), 
                            octdec(str_pad($iterator->getPerms(), 4, 0, STR_PAD_LEFT)), false, $context);
                } else {
                    if (empty($context))
                        $this->copy($item, $dest . DIRECTORY_SEPARATOR . $iterator->getSubPathName());
                    else
                        $this->copy($item, $dest . DIRECTORY_SEPARATOR . $iterator->getSubPathName(), 
                            $context);
                }
            }
            
            return true;
        } else
            return false;
    }

    /**
     *
     * {@inheritDoc}
     *
     */
    public function mv($oldname = false, $newname = false, $context = false)
    {
        if (empty($oldname) || empty($newname))
            return false;
        
        $oldname = $this->makeAbsolute($oldname);
        $newname = $this->makeAbsolute($newname);
        
        if (! empty($context))
            return rename($oldname, $newname, $context);
        
        return rename($oldname, $newname);
    }

    /**
     *
     * {@inheritDoc}
     *
     */
    public function namePatternMatch($pattern = false, $string = false, $flags = false)
    {
        if (empty($pattern))
            return false;
        
        if (! empty($flags))
            return fnmatch($pattern, $string, $flags);
        else
            return fnmatch($pattern, $string);
    }

    /**
     *
     * {@inheritDoc}
     *
     */
    public function getGlob($pattern = false, $flags = 0)
    {
        return glob($pattern, $flags);
    }

    /**
     *
     * {@inheritDoc}
     *
     */
    public function isUploadedFile($filename = false)
    {
        $filename = $this->makeAbsolute($filename);
        return is_uploaded_file($filename);
    }

    /**
     *
     * {@inheritDoc}
     *
     */
    public function getRealpathCache()
    {
        return realpath_cache_get();
    }

    /**
     *
     * {@inheritDoc}
     *
     */
    public function getRealpathCacheSize()
    {
        return realpath_cache_size();
    }

    /**
     *
     * {@inheritDoc}
     *
     */
    public function close($keyword = false)
    {
        if (!empty($keyword) && array_key_exists($keyword, $this->dirkeys) &&
             array_key_exists($this->dirkeys[$keyword], $this->dirs)) {
            unset($this->dirs[$this->dirkeys[$keyword]]);
            unset($this->dirkeys[$keyword]);
            return true;
        } elseif(!empty($keyword) && array_key_exists($keyword,$this->files)) {
            $this->files[$keyword] = null;
            unset($this->files[$keyword]);
            return true;
        } else {
            return false;
        }
    }

    /**
     *
     * {@inheritDoc}
     *
     */
    public function touch($filename = false, $time = false, $atime = false)
    {
        if (empty($filename))
            return false;
        
        $filename = $this->makeAbsolute($filename);
        
        $time = ! empty($time) ? $time : time();
        $atime = ! empty($atime) ? $atime : time();
        
        return touch($filename, $time, $atime);
    }

    /**
     *
     * {@inheritDoc}
     *
     */
    public function getDirKeys()
    {
        return $this->dirkeys;
    }

    /**
     *
     * {@inheritDoc}
     *
     */
    public function getFileKeys()
    {
        return $this->files;
    }
    
    /**
     *
     * {@inheritDoc}
     *
     */
    public function createStructure($rootpath, $data_array, $skip_debug = false)
    {
        $absolute_path = $this->makeAbsolute($rootpath);
        if (empty($skip_debug))
            $this->debug(810);
        if (empty($rootpath) || ! is_array($data_array) ||
             (! $this->isWritable($absolute_path) && ! $this->isWritable(dirname($absolute_path))))
            return $this->warning(506) && false;
        
        if (! $this->isDir($absolute_path))
            mkdir($absolute_path, 0755, true);
        
        foreach ($data_array as $key => $val) {
            if (is_array($val))
                ! $this->createStructure($absolute_path . DIRECTORY_SEPARATOR . $key, $val, true);
            else
                touch($absolute_path . DIRECTORY_SEPARATOR . $val);
        }
        if (empty($skip_debug))
            $this->info(704, $rootpath);
        return true;
    }
}
