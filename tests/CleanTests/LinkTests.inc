<?php
use HOWI3\libhowi\Filesystem\FS;

class LinkTests extends PHPUnit_Framework_TestCase
{

    protected $FS;

    protected function setUp()
    {
        if ($this->FS instanceof HOWI3\libhowi\Filesystem\FS)
            return;
        
        $this->FS = FS::load();
        $logfile = $this->FS->makeAbsolute(FS_TEST_LOG_LOCATION);
        $this->FS->setLogLevel(FS_TEST_LOG_LEVEL);
        $this->FS->setUID(getmyuid());
        $this->FS->setUsername(get_current_user());
        $this->FS->setLogFile($logfile);
    }

    protected function addWarning($msg, Exception $previous = null)
    {
        $add_warning = $this->getTestResultObject();
        $msg = new PHPUnit_Framework_Warning($msg, 0, $previous);
        $add_warning->addWarning($this, $msg, time());
        $this->setTestResultObject($add_warning);
    }
    
    public function test_link()
    {
        
        $this->assertTrue($this->FS->link()->hardlink('~/HOWI3/tmp/libhowi-filesystem/documents/test_file.txt',
            '~/HOWI3/tmp/libhowi-filesystem/documents/test_file_hardlink.txt'));
        /* No hardlink on directories */
        $this->assertFalse($this->FS->link()->hardlink('~/HOWI3/tmp/libhowi-filesystem/documents','~/HOWI3/tmp/libhowi-filesystem/documents-hl'));
        
        $this->assertTrue($this->FS->link()->symlink('~/HOWI3/tmp/libhowi-filesystem/documents/test_file.txt', 
            '~/HOWI3/tmp/libhowi-filesystem/documents/test_file_symlink.txt'));
    }
    
    public function test_lstat()
    {
        $this->assertInternalType('array',$this->FS->link()->lstat('~/HOWI3/tmp/libhowi-filesystem/documents/test_file_hardlink.txt'));
    
        $this->assertFalse($this->FS->link()->lstat('~/HOWI3/tmp/libhowi-filesystem/documents-hl'));
    
    }
    
    public function test_lstatDiff()
    {
        $this->assertInternalType('array',$this->FS->link()->lstatDiff('~/HOWI3/tmp/libhowi-filesystem/documents/test_file_hardlink.txt'));
    
    
    }
    
    public function test_readlink()
    {
        $this->assertInternalType('string',$this->FS->link()->readlink('~/HOWI3/tmp/libhowi-filesystem/documents/test_file_symlink.txt'));
    }
    
    public function test_lchgrp()
    {
       
        $this->assertFalse($this->FS->link()->lchgrp());
        
        $userID = getmyuid();
        if($userID) {
            $this->assertTrue($this->FS->link()->lchgrp('~/HOWI3/tmp/libhowi-filesystem/documents/test_file_symlink.txt',getmygid()));
        } else {
            $this->markTestIncomplete();
        }
        
    }
    
    public function test_lchown()
    {
        $userID = getmyuid();
        if($userID) {
            $this->assertFalse($this->FS->link()->lchown());
            $this->assertTrue($this->FS->link()->lchown('~/HOWI3/tmp/libhowi-filesystem/documents/test_file_symlink.txt',getmyuid()));
        } else {
            $this->markTestSkipped();
        }
        
    
    }
    
    public function test_linkinfo()
    {
        $this->assertFalse($this->FS->link()->linkinfo());
        $this->assertInternalType('int',$this->FS->link()->linkinfo('~/HOWI3/tmp/libhowi-filesystem/documents/test_file_symlink.txt'));
    
    }
    public function test_functions()
    {
        $methods = get_class_methods($this->FS->link());
    
        foreach ($methods as $method) {
            $skip = array(
                '__construct',
                'symlink',
                'hardlink',
                'getSize',
                'getATime',
                'getCTime',
                'getMTime',
                'ct_ago',
                'openFile',
                'setFileClass',
                'setInfoClass',
                'getOwnerName',
                'getGroupName',
                'getRealpath',
                'getPathInfo',
                'getGroup',
                'getOwner',
                'getInode',
                'getBasename',
                'getPerms',
                'getType',
                'isDot',
                'isAbsolute',
                'isRelative',
                'isWritable',
                'isReadable',
                'isDir',
                'isExecutable',
                'isFile',
                'isLink',
                'makeAbsolute',
                'exists',
                'getCwd',
                'getFilename',
                'getExtension',
                'getPath',
                'getPathname'
            );
            if (! in_array($method, $skip) && ! method_exists($this, "test_$method")) {
                $this->addWarning(
                    "Method $method has no test_$method! Implement test or add method into 'skip' array");
    
               // break;
            }
        }
        $this->assertNull(null);
    }
    
    protected function tearDown()
    {
        unset($this->FS);
    }
}