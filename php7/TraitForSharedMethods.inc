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
 * File:      TraitForSharedMethods.inc
 * @link      https://
 ********************************************************************
 * Contributors:
 * @author Marko Kungla <okramlabs@gmail.com>
 *           Github: https://github.com/mkungla
 ********************************************************************
 * Comments:
 */
namespace HOWI3\libhowi\Filesystem\php7;

use \RecursiveDirectoryIterator;
use \RecursiveIteratorIterator;
use \DateTime;

trait TraitForSharedMethods
{

    /**
     *
     * {@inheritDoc}
     *
     */
    protected function setCwd($path = false, $validate_dir_name = false)
    {
        if (empty($path))
            $current_working_directory = $this->getCwd();
        else
            $current_working_directory = $this->makeAbsolute($path);
        
        if (! empty($validate_dir_name) && (basename($current_working_directory) !== $validate_dir_name))
            return false;
        
        if (! is_dir($current_working_directory) || ! is_readable($current_working_directory) ||
             ! chdir($current_working_directory)) {
            $this->alert(200, false, $current_working_directory);
            return false;
        }
        $this->cwd = $current_working_directory;
        return true;
    }

    /**
     *
     * {@inheritDoc}
     *
     */
    public function getCwd()
    {
        if (empty($this->cwd))
            $this->cwd = getcwd();
        
        return $this->cwd;
    }

    /**
     *
     * {@inheritDoc}
     *
     */
    public function getSize($convert = false, $filename = false)
    {
        $filename = ! empty($filename) ? $this->makeAbsolute($filename) : false;
        $bytestotal = empty($filename) ? ((method_exists(get_parent_class($this), 'getSize')) ? parent::getSize() : false) : filesize(
            $filename);
        
        if (! $this->isDir($filename) && empty($convert))
            return $bytestotal;
        
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
        if ($this->isDir($filename)) {
            $path = ((method_exists(get_class($this), 'getPathname')) ? $this->getPathname() : $filename);
            if ($path === '/home/nitrotrigger/tmp/libhowi-filesystem/log/phpunit_test.log')
                die('ss');
            
            foreach (new RecursiveIteratorIterator(
                new RecursiveDirectoryIterator($path, RecursiveDirectoryIterator::SKIP_DOTS)) as $object) {
                $bytestotal += $object->getSize();
            }
            if (! $convert)
                return $bytestotal;
        }
        $base = log($bytestotal, 1024);
        
        $result = [
            0,
            'B'
        ];
        if ($bytestotal > 0) {
            
            $result = array(
                round(pow(1024, $base - floor($base)), 2),
                $suffixes[floor($base)]
            );
        }
        return ! empty($convert) ? $result : $result[0];
    }

    /**
     *
     * {@inheritDoc}
     *
     */
    public function getATime($timeformat = false, $pathname = false)
    {
        $atime = empty($pathname) ? parent::getATime() : fileatime($this->makeAbsolute($pathname));
        if (! empty($timeformat) && is_int($atime)) {
            switch ($timeformat) {
                case 'ago':
                    $atime = $this->ct_ago($atime, 'y,m,d,h,i,s', false, true);
                    break;
                case 'ago.single':
                    $atime = $this->ct_ago($atime, 'y,m,d,h,i,s', false, true, true);
                    break;
                default:
                    $atime = date($timeformat, $atime);
                    break;
            }
        }
        return $atime;
    }

    /**
     *
     * {@inheritDoc}
     *
     */
    public function getCTime($timeformat = false, $pathname = false)
    {
        $ctime = empty($pathname) ? parent::getCTime() : filectime($this->makeAbsolute($pathname));
        if (! empty($timeformat) && is_int($ctime)) {
            switch ($timeformat) {
                case 'ago':
                    $ctime = $this->ct_ago($ctime, 'y,m,d,h,i,s', false, true);
                    break;
                case 'ago.single':
                    $ctime = $this->ct_ago($ctime, 'y,m,d,h,i,s', false, true, true);
                    break;
                default:
                    $ctime = date($timeformat, $ctime);
                    break;
            }
        }
        return $ctime;
    }

    /**
     *
     * {@inheritDoc}
     *
     */
    public function getMTime($timeformat = false, $pathname = false)
    {
        $mtime = empty($pathname) ? parent::getMTime() : filemtime($this->makeAbsolute($pathname));
        if (! empty($timeformat) && is_int($mtime)) {
            switch ($timeformat) {
                case 'ago':
                    $mtime = $this->ct_ago($mtime, 'y,m,d,h,i,s', false, true);
                    break;
                case 'ago.single':
                    $mtime = $this->ct_ago($mtime, 'y,m,d,h,i,s', false, true, true);
                    break;
                default:
                    $mtime = date($timeformat, $mtime);
                    break;
            }
        }
        return $mtime;
    }

    /**
     *
     * {@inheritDoc}
     *
     */
    public function ct_ago($timestamp, $date_format = 'y,m,d,h,i,s', $sfx = true, $ommit_zero = true, $ago_single = true, $clocale = [])
    {
        $locale = array_merge(
            array(
                'y' => array(
                    'y',
                    'year',
                    'years'
                ),
                'm' => array(
                    'm',
                    'month',
                    'months'
                ),
                'd' => $d = array(
                    'd',
                    'day',
                    'days'
                ),
                'h' => array(
                    'h',
                    'hour',
                    'hours'
                ),
                'i' => array(
                    'm',
                    'minute',
                    'minutes'
                ),
                's' => array(
                    's',
                    'second',
                    'seconds'
                ),
                'days' => $d,
                'ago' => 'ago',
                'now' => 'just now'
            ), $clocale);
        
        $date = new DateTime();
        $date->setTimestamp($timestamp);
        
        $interval = $date->diff(new DateTime('now'));
        
        for ($df = strtok($date_format, ','); $df !== false; $df = strtok(",")) {
            $lkey = ($df !== 'a') ? strtolower($df) : 'days';
            
            if ($ommit_zero && $interval->$lkey === 0)
                continue;
            
            $fparam = $df . (empty($sfx) ? $locale[$lkey][0] : ' ' .
                 (($interval->$lkey === 1) ? $locale[$lkey][1] : $locale[$lkey][2]));
            
            if (! empty($ago_single) && ! empty($dfs))
                break;
            else
                $dfs = ! empty($dfs) ? $dfs . "%$fparam " : "%$fparam ";
        }
        if (empty($dfs))
            $ret = $locale['now'];
        else
            $ret = $interval->format($dfs) . $locale['ago'];
        return $ret;
    }

    /**
     *
     * {@inheritDoc}
     *
     */
    public function openFile($open_mode = "r", $use_include_path = false, $context = null)
    {
        if (! method_exists(get_parent_class($this), 'openFile'))
            return false;
        
        return ! $this->isDir() ? (! empty($context) ? parent::openFile($open_mode, $use_include_path, 
            $context) : parent::openFile($open_mode, $use_include_path)) : false;
    }

    /**
     *
     * {@inheritDoc}
     *
     */
    public function setFileClass($class_name = "\HOWI3\libhowi\Filesystem\php7\Objects\FileObject")
    {
        if (! method_exists(get_parent_class($this), 'setFileClass'))
            return false;
        
        if (is_subclass_of($class_name, 'SplFileObject') || $class_name === 'SplFileObject') {
            parent::setFileClass($class_name);
            return true;
        }
        return false;
    }

    /**
     *
     * {@inheritDoc}
     *
     */
    public function setInfoClass($class_name = "\HOWI3\libhowi\Filesystem\php7\Objects\InfoObject")
    {
        if (! method_exists(get_parent_class($this), 'setInfoClass'))
            return false;
        
        if (is_subclass_of($class_name, 'SplFileInfo') || $class_name === 'SplFileInfo') {
            parent::setInfoClass($class_name);
            return true;
        }
        return false;
    }

    /**
     *
     * {@inheritDoc}
     *
     */
    public function getOwnerName($filename = false)
    {
        $userID = empty($filename) && method_exists(get_parent_class($this), 'getOwner') ? parent::getOwner() : (! empty(
            $filename) ? fileowner($this->makeAbsolute($filename)) : false);
        $user = ! empty($userID) ? posix_getpwuid($userID) : false;
        return is_array($user) && array_key_exists('name', $user) ? $user['name'] : false;
    }

    /**
     *
     * {@inheritDoc}
     *
     */
    public function getGroupName($filename = false)
    {
        $groupID = empty($filename) ? ((method_exists(get_parent_class($this), 'getGroup')) ? parent::getGroup() : false) : filegroup(
            $this->makeAbsolute($filename));
        $group = ! empty($groupID) ? posix_getgrgid($groupID) : false;
        return ! empty($group) && array_key_exists('name', $group) ? $group['name'] : false;
    }

    /**
     *
     * {@inheritDoc}
     *
     */
    public function getRealpath($filename = false)
    {
        return ! empty($filename) ? realpath($this->makeAbsolute($filename)) : ((method_exists(
            get_parent_class($this), 'getRealpath')) ? parent::getRealpath() : false);
    }

    /**
     *
     * {@inheritDoc}
     *
     */
    public function getPathInfo($filename = false, $flags = false, 
        $class_name = '\HOWI3\libhowi\Filesystem\php7\Objects\InfoObject')
    {
        return ! empty($filename) ? empty($flags) ? pathinfo($this->makeAbsolute($filename)) : pathinfo(
            $this->makeAbsolute($filename), $flags) : (method_exists(get_parent_class($this), 'getPathInfo') ? parent::getPathInfo(
            $class_name) : false);
    }

    /**
     *
     * {@inheritDoc}
     *
     */
    public function getPath()
    {
        return method_exists(get_parent_class($this), 'getPath') ? parent::getPath() : (property_exists($this, 
            'path') ? $this->path : dirname($this->getPathname()));
    }

    /**
     *
     * {@inheritDoc}
     *
     */
    public function getPathname()
    {
        return method_exists(get_parent_class($this), 'getPathname') ? parent::getPathname() : $this->getCwd();
    }

    /**
     *
     * {@inheritDoc}
     *
     */
    public function getGroup($filename = false)
    {
        return empty($filename) ? ((method_exists(get_parent_class($this), 'getGroup')) ? parent::getGroup() : false) : filegroup(
            $this->makeAbsolute($filename));
    }

    /**
     *
     * {@inheritDoc}
     *
     */
    public function getOwner($filename = false)
    {
        return empty($filename) ? ((method_exists(get_parent_class($this), 'getOwner')) ? parent::getOwner() : false) : fileowner(
            $this->makeAbsolute($filename));
    }

    /**
     *
     * {@inheritDoc}
     *
     */
    public function getInode($filename = false)
    {
        return empty($filename) ? ((method_exists(get_parent_class($this), 'getInode')) ? parent::getInode() : false) : fileinode(
            $this->makeAbsolute($filename));
    }

    /**
     *
     * {@inheritDoc}
     *
     */
    public function getBasename($suffix = false, $pathname = false)
    {
        $pathname = ! empty($pathname) ? $pathname : ($this->isDot() ? $this->getPath() : false);
        $suffix = ! empty($suffix) ? $suffix : '';
        return ! empty($pathname) || ! method_exists(get_parent_class($this), 'getBasename') ? basename(
            $pathname, $suffix) : parent::getBasename($suffix);
    }

    /**
     *
     * {@inheritDoc}
     *
     */
    public function getPerms($filename = false)
    {
        $perms = empty($filename) ? ((method_exists(get_parent_class($this), 'getPerms')) ? parent::getPerms() : false) : fileperms(
            $this->makeAbsolute($filename));
        return ! empty($perms) ? (int) ltrim(substr(sprintf('%o', $perms), - 4), '0') : false;
    }

    /**
     *
     * {@inheritDoc}
     *
     */
    public function getType($filename = false)
    {
        return ! empty($filename) ? filetype($this->makeAbsolute($filename)) : ((method_exists(
            get_parent_class($this), 'getType')) ? parent::getType() : false);
    }

    /**
     *
     * {@inheritDoc}
     *
     */
    public function isDot()
    {
        return ((method_exists(get_parent_class($this), 'isDot')) ? parent::isDot() : false);
    }

    /**
     *
     * {@inheritDoc}
     *
     */
    public function isAbsolute($path = false)
    {
        return ((strspn($path, DIRECTORY_SEPARATOR, 0, 1) + (ctype_alnum($path) ? 0 : 1) +
             (strpos($path, ':') === false) + (strpos($path, "/../") === false ? 1 : 0) +
             (filter_var($path, FILTER_VALIDATE_URL) === false ? 1 : 0) === 5) ? true : false);
    }

    /**
     *
     * {@inheritDoc}
     *
     */
    public function isRelative($path = false)
    {
        // ^(?=.*?(.))((?!(^\/)).)*$
        return (preg_match('/^[^\/].*$/', $path) === 1) ? true : false;
    }

    /**
     *
     * {@inheritDoc}
     *
     */
    public function isWritable($path = false)
    {
        return empty($path) ? ((method_exists(get_parent_class($this), 'isWritable')) ? parent::isWritable() : false) : is_writable(
            $this->makeAbsolute($path));
    }

    /**
     *
     * {@inheritDoc}
     *
     */
    public function isReadable($path = false)
    {
        return empty($path) ? ((method_exists(get_parent_class($this), 'isReadable')) ? parent::isReadable() : false) : is_readable(
            $this->makeAbsolute($path));
    }

    /**
     *
     * {@inheritDoc}
     *
     */
    public function isDir($filename = false)
    {
        return empty($filename) ? ((method_exists(get_parent_class($this), 'isDir')) ? parent::isDir() : false) : is_dir(
            $this->makeAbsolute($filename));
    }

    /**
     *
     * {@inheritDoc}
     *
     */
    public function isExecutable($filename = false)
    {
        return ! empty($filename) ? is_executable($this->makeAbsolute($filename)) : ((method_exists(
            get_parent_class($this), 'isExecutable')) ? parent::isExecutable() : false);
    }

    /**
     *
     * {@inheritDoc}
     *
     */
    public function isFile($filename = false)
    {
        return ! empty($filename) ? is_file($this->makeAbsolute($filename)) : ((method_exists(
            get_parent_class($this), 'isFile')) ? parent::isFile() : false);
    }

    /**
     *
     * {@inheritDoc}
     *
     */
    public function isLink($filename = false)
    {
        return ! empty($filename) ? is_link($this->makeAbsolute($filename)) : ((method_exists(
            get_parent_class($this), 'isLink')) ? parent::isLink() : false);
    }

    /**
     *
     * {@inheritDoc}
     *
     */
    public function makeAbsolute($path = false)
    {
        if (! empty($path) && $this->isAbsolute($path))
            $absolute_path = $path;
        elseif (! empty($path) && $this->isRelative($path)) {
            
            if (preg_match('/^(~\/)/', $path) === 1) {
                
                $userinfo = posix_getpwuid(getmyuid());
                
                if (array_key_exists('dir', $userinfo)) {
                    $absolute_path = $userinfo['dir'] . substr($path, 1);
                }
            } elseif (preg_match('/^(.\/|..\/)/', $path) === 1) {
                $absolute_path = realpath($path);
            } else
                $absolute_path = $this->getCwd() . ($path !== '.' ? DIRECTORY_SEPARATOR . $path : '');
        } else {
            $absolute_path = $path;
        }
        return $absolute_path;
    }

    /**
     *
     * {@inheritDoc}
     *
     */
    public function exists($pathname = false)
    {
        if (empty($pathname))
            return false;
        
        $pathname = $this->makeAbsolute($pathname);
        
        return ! empty($pathname) ? file_exists($pathname) : false;
    }

    /**
     *
     * {@inheritDoc}
     *
     */
    public function getFilename($filename = false)
    {
        return ! empty($filename) ? basename($this->makeAbsolute($filename)) : ((method_exists(
            get_parent_class($this), 'getFilename')) ? parent::getFilename() : false);
    }

    /**
     *
     * {@inheritDoc}
     *
     */
    public function getExtension($filename = false)
    {
        return ! empty($filename) ? pathinfo($this->makeAbsolute($filename), PATHINFO_EXTENSION) : ((method_exists(
            get_parent_class($this), 'getExtension')) ? parent::getExtension() : false);
    }
}
