<?php
/********************************************************************
 * Created by:	Marko Kungla @ OkramLabs on Aug 6, 2012 - 9:27:39
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
 * File:		FS.inc
 * @link		https://
 ********************************************************************
 * Contributors:
 * @author		Marko Kungla <okramlabs@gmail.com>
 *  	Github:	https://github.com/mkungla
 ********************************************************************
 * Comments:
 */
namespace HOWI3\libhowi\Filesystem;

class FS
{
    use \HOWI3\libhowi\Filesystem\Commons\TraitForResponse;

    /**
     * Initialize the filesystem
     *
     * @name $FS = FS::load( string $pathname | $ser_cwd_by_caller boolean true = false, PHP_VERSION_ID );
     *      
     * @param string|boolean $pathname|$ser_cwd_by_caller
     *            Optional: defaults to false.
     *            If path is supplied then that will be current working directory.
     *            If boolean true is supplied it will try to set current working directory as one
     *            containing current script where FS::load() was called.
     * @return Filesystem Returns filesystem object.
     */
    public static function load($ser_cwd_by_caller = false, $PHP_VERSION_ID = PHP_VERSION_ID)
    {
        if (! empty($ser_cwd_by_caller) && is_bool($ser_cwd_by_caller)) {
            $backtrace = debug_backtrace(
                defined("DEBUG_BACKTRACE_IGNORE_ARGS") ? DEBUG_BACKTRACE_IGNORE_ARGS : FALSE);
            $cwd_caller = array_pop($backtrace);
            $ser_cwd_by_caller = dirname($cwd_caller['file']);
        }
        /**
         * Minimum version of PHP must be atleast 5.5.11
         */
        if ($PHP_VERSION_ID <= 50511)
            return call_user_func(
                function () {
                    $self = new self();
                    $self->emergency(100);
                    return $self;
                });
        
        $filesystem = $PHP_VERSION_ID >= 70000 ? 'HOWI3\libhowi\Filesystem\php7\Filesystem' : 'HOWI3\libhowi\Filesystem\php5\Filesystem';
        /**
         * Load APP writen in PHP 7 flavor or
         * Compatible PHP v5.4 > && < PHP v7
         */
        return new $filesystem($ser_cwd_by_caller);
    }
}
