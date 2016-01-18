<?php
use HOWI3\libhowi\Filesystem\FS;

class TmpTests extends PHPUnit_Framework_TestCase
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
    
    public function test_tmp()
    {
        
    
        $this->assertInternalType('string',$this->FS->tmp()
            ->getTmp());
        $this->assertTrue($this->FS->tmp()
            ->setTmp());
        $this->assertTrue($this->FS->tmp()
            ->setTmp('/tmp'));
        
        $this->FS->dir('tmp',FS_TEST_LOCATION);
        $this->assertTrue($this->FS->tmp()
            ->setTmp('~/HOWI3/tmp/libhowi-filesystem/tmp'));
        $this->assertInternalType('string',$this->FS->tmp()
            ->getTmp());
    }
    
    public function test_tempnam()
    {
        
        $this->assertTrue($this->FS->tmp()
        ->setTmp('~/HOWI3/tmp/libhowi-filesystem/tmp'));
        
      
        
        $this->assertInternalType('string', $this->FS->tmp()
            ->tempnam());
        $this->assertTrue($this->FS->isAbsolute($this->FS->tmp()
            ->tempnam()));
        $this->assertTrue($this->FS->isAbsolute($this->FS->tmp()
            ->tempnam()));
    }
    
    public function test_tmpfile()
    {
        $this->assertTrue($this->FS->tmp()
        ->setTmp('~/HOWI3/tmp/libhowi-filesystem/tmp'));
        
        $tmpfile = $this->FS->tmp()->tmpfile();
        $this->assertInternalType('resource',$tmpfile);
        fclose($tmpfile);
    }
    
    public function test_functions()
    {
        $methods = get_class_methods($this->FS->tmp());
    
        foreach ($methods as $method) {
            $skip = array(
                '__construct',
                'setTmp',
                'getTmp',
                'setLogLevel',
                'setLogFile',
                'getLogFile',
                'getLogLevel',
                'getUID',
                'getTime',
                'getUsername',
                'response',
                'getStatus',
                'getMsg',
                'getCode',
                'setUID',
                'setUsername',
                'setStatus',
                'setCode',
                'emergency',
                'alert',
                'critical',
                'error',
                'warning',
                'notice',
                'info',
                'debug',
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
    
                break;
            }
        }
        $this->assertNull(null);
    }
    
    protected function tearDown()
    {
        unset($this->FS);
    }
}