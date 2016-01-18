<?php
/*******************************************************************
 * Created by:  Marko Kungla @ OkramLabs on aug 6, 2012 - 9:11:06
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
 * File:      TmpTrait.inc
 * @link      https://
 ********************************************************************
 * Contributors:
 * @author Marko Kungla <okramlabs@gmail.com>
 *           Github: https://github.com/mkungla
 ********************************************************************
 * Comments:
 */
namespace HOWI3\libhowi\Filesystem\php7\Objects;

use HOWI3\libhowi\Filesystem\Commons\ObjectInterfaces\TmpInterface;
use \HOWI3\libhowi\Filesystem\Commons\TraitForResponse;
use \HOWI3\libhowi\Filesystem\php7\TraitForSharedMethods;

class TmpObject implements TmpInterface
{
    
    use TraitForResponse;
    use TraitForSharedMethods;

    private $tmp_path;

    /**
     *
     * {@inheritDoc}
     *
     */
    public function setTmp($path = false)
    {
        if (! empty($path)) {
            $tmp = $this->makeAbsolute($path);
            $isWritable = $this->isWritable($tmp);
            $this->tmp_path = $isWritable ? $tmp : $this->tmp_path;
            
            ! empty($isWritable) ? $this->debug(803) : $this->warning(501, $tmp);
            $this->response()->setStatus($isWritable);
        } else {
            $this->tmp_path = sys_get_temp_dir();
            
            $this->debug(804);
            $this->response()->setStatus(true);
            $isWritable = true;
        }
        return $isWritable;
    }

    /**
     *
     * {@inheritDoc}
     *
     */
    public function getTmp()
    {
        if (empty($this->tmp_path))
            $this->tmp_path = $this->setTmp();
        
        $this->debug(805, $this->tmp_path);
        $this->response()->setStatus(true);
        return $this->tmp_path;
    }

    /**
     *
     * {@inheritDoc}
     *
     */
    public function tempnam($dir = false, $prefix = 'howi-fs-tmp-')
    {
        $dir = empty($dir) ? $this->getTmp() : $this->makeAbsolute($dir);
        
        $tmpname = tempnam($dir, $prefix);
        
        $this->debug(806, $tmpname);
        $this->response()->setStatus(true);
        
        return $tmpname;
    }

    /**
     *
     * {@inheritDoc}
     *
     */
    public function tmpfile()
    {
        $this->debug(807);
        $tmpfie = tmpfile();
        $this->response()->setStatus(! empty($tmpfie) ? true : false);
        return $tmpfie;
    }
}
