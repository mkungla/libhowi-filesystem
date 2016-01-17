<?php
/********************************************************************
 * Created by:	Marko Kungla @ OkramLabs on Aug 6, 2012 - 9:11:26
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
 * File:		AbstractFilesystem.inc
 * @link		https://
 ********************************************************************
 * Contributors:
 * @author		Marko Kungla <okramlabs@gmail.com>
 *  	Github:	https://github.com/mkungla
 ********************************************************************
 * Comments:
 */
namespace HOWI3\libhowi\Filesystem\Commons;

abstract class AbstractFilesystem
{

    /**
     *
     * @var string $cwd current working directory.
     * @var array $dirkeys Reqistered basenames
     * @var array $dirs DirectoryTreeObjects
     * @var array $files FileObjects by basename
     * @var array $infos InfoObjects by basename
     * @var object $tmp tmpObject if loaded
     * @var object $link linkObject if loaded
     */
    protected $cwd;

    protected $dirkeys = [];

    protected $dirs = [];

    protected $files = [];

    protected $infos = [];

    protected $tmp = [];

    protected $link = [];

    /**
     * setCwd — Sets the current working directory
     *
     * @return void;
     */
    abstract protected function setCwd($path = false, $validate_dir_name = false);
}
