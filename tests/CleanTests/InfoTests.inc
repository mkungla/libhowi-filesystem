<?php
use HOWI3\libhowi\Filesystem\FS;
use HOWI3\libhowi\Filesystem\php5\Objects\InfoObject;

class InfoTests extends PHPUnit_Framework_TestCase
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

    public function test_info()
    {
        $this->assertFalse($this->FS->infoObject());
        $this->assertEquals(602, $this->FS->getCode());
        $this->assertFalse($this->FS->getStatus());
        
        $this->assertFalse($this->FS->infoObject('test_file.txt', '/'));
        $this->assertEquals(504, $this->FS->getCode());
        $this->assertFalse($this->FS->getStatus());
        
        $this->assertFalse($this->FS->infoObject('test_file.txt', '/root'));
        $this->assertEquals(504, $this->FS->getCode());
        $this->assertFalse($this->FS->getStatus());
        
        $this->assertInstanceOf('HOWI3\libhowi\Filesystem\Commons\ObjectInterfaces\InfoInterface', 
            $this->FS->infoObject('test_file.txt', '~/HOWI3/tmp/libhowi-filesystem/documents'));
        $this->assertEquals(809, $this->FS->getCode());
        $this->assertFalse($this->FS->getStatus());
    }

    public function test_getLinkTarget()
    {
    
        $this->FS->infoObject('test_file.txt', '~/HOWI3/tmp/libhowi-filesystem/documents');
    
        $this->assertFalse($this->FS->infoObject('test_file.txt')
            ->getLinkTarget());
    }

    public function test_getPath()
    {
    
        $this->FS->infoObject('test_file.txt', '~/HOWI3/tmp/libhowi-filesystem/documents');
    
        $this->assertEquals(getenv('HOME') . '/HOWI3/tmp/libhowi-filesystem/documents',
            $this->FS->infoObject('test_file.txt')
            ->getPath(), 'Failed to assert getPath');
    }
    
    public function test_getFilename()
    {
    
        $this->FS->infoObject('test_file.txt', '~/HOWI3/tmp/libhowi-filesystem/documents');
    
        $this->assertEquals('test_file.txt', $this->FS->infoObject('test_file.txt')
            ->getFilename(), '~/HOWI3/tmp/libhowi-filesystem/log/phpunitTestLog.txt should be string txt');
    }
    
    public function test_getExtension()
    {
    
        $this->FS->infoObject('test_file.txt', '~/HOWI3/tmp/libhowi-filesystem/documents');
    
        $this->assertEquals('txt', $this->FS->infoObject('test_file.txt')
            ->getExtension(), '~/HOWI3/tmp/libhowi-filesystem/log/test_file.txt should be string txt');
    }
    
    public function test_getBasename()
    {
    
        $this->FS->infoObject('test_file.txt', '~/HOWI3/tmp/libhowi-filesystem/documents');
    
        $this->assertEquals('test_file.txt', $this->FS->infoObject('test_file.txt')
            ->getBasename());
    }
    
    public function test_getPathName()
    {
    
        $this->FS->infoObject('test_file.txt', '~/HOWI3/tmp/libhowi-filesystem/documents');
    
        $this->assertEquals(getenv('HOME') . '/HOWI3/tmp/libhowi-filesystem/documents/test_file.txt',
            $this->FS->infoObject('test_file.txt')
            ->getPathName(), 'Failed to assert getPathName');
    }
    
    public function test_getPerms()
    {
    
        $this->FS->infoObject('test_file.txt', '~/HOWI3/tmp/libhowi-filesystem/documents');
    
        $this->assertEquals(664, $this->FS->infoObject('test_file.txt')
            ->getPerms(), ' permissions should be 664');
    }
    
    public function test_getInode()
    {
    
        $this->FS->infoObject('test_file.txt', '~/HOWI3/tmp/libhowi-filesystem/documents');
    
        $this->assertInternalType('int', $this->FS->infoObject('test_file.txt')
            ->getInode());
    }
    
    public function test_getSize()
    {
    
        $this->FS->infoObject('test_file.txt', '~/HOWI3/tmp/libhowi-filesystem/documents');
    
        $this->assertInternalType('int', $this->FS->infoObject('test_file.txt')
            ->getSize());
        $this->assertInternalType('array', $this->FS->infoObject('test_file.txt')
            ->getSize(true));
    }
    
    public function test_getOwner()
    {
    
        $this->FS->infoObject('test_file.txt', '~/HOWI3/tmp/libhowi-filesystem/documents');
    
        $userID = getmyuid();
        $this->assertInternalType('int', $this->FS->infoObject('test_file.txt')
            ->getOwner());
        $this->assertEquals($userID, $this->FS->infoObject('test_file.txt')
            ->getOwner(), ' owner should match with current users ID');
    }
    
    public function test_getGroup()
    {
    
        $this->FS->infoObject('test_file.txt', '~/HOWI3/tmp/libhowi-filesystem/documents');
    
        $group = getmygid();
    
        $this->assertEquals($group, $this->FS->infoObject('test_file.txt')
            ->getGroup(), ' group should match with current users group');
    }
    
    public function test_getATime()
    {
    
        $this->FS->infoObject('test_file.txt', '~/HOWI3/tmp/libhowi-filesystem/documents');
    
        $this->assertInternalType('int', $this->FS->infoObject('test_file.txt')
            ->getATime(), 'Default getAccessTime must be a int');
        $this->assertInternalType('string', $this->FS->infoObject('test_file.txt')
            ->getATime('ago'), ' getAccessTime must be a string');
        $this->assertInternalType('string',
            $this->FS->infoObject('test_file.txt')
            ->getATime('ago.single'), ' getAccessTime must be a string');
        $this->assertInternalType('string', $this->FS->infoObject('test_file.txt')
            ->getATime('c'), ' getAccessTime must be a string');
    }
    
    public function test_getMTime()
    {
    
        $this->FS->infoObject('test_file.txt', '~/HOWI3/tmp/libhowi-filesystem/documents');
    
        $this->assertInternalType('int', $this->FS->infoObject('test_file.txt')
            ->getMTime(), 'Default getModifTime must be a int');
        $this->assertInternalType('string', $this->FS->infoObject('test_file.txt')
            ->getMTime('ago'), ' getModifTime must be a string');
        $this->assertInternalType('string',
            $this->FS->infoObject('test_file.txt')
            ->getMTime('ago.single'), ' getModifTime must be a string');
        $this->assertInternalType('string', $this->FS->infoObject('test_file.txt')
            ->getMTime('c'), ' getModifTime must be a string');
    }
    
    public function test_getCTime()
    {
    
        $this->FS->infoObject('test_file.txt', '~/HOWI3/tmp/libhowi-filesystem/documents');
    
        $this->assertInternalType('int', $this->FS->infoObject('test_file.txt')
            ->getCTime(), 'Default getAccessTime must be a int');
        $this->assertInternalType('string', $this->FS->infoObject('test_file.txt')
            ->getCTime('ago'), ' getAccessTime must be a string');
        $this->assertInternalType('string',
            $this->FS->infoObject('test_file.txt')
            ->getCTime('ago.single'), ' getAccessTime must be a string');
        $this->assertInternalType('string', $this->FS->infoObject('test_file.txt')
            ->getCTime('c'), ' getAccessTime must be a string');
    }
    
    public function test_getType()
    {
    
        $this->FS->infoObject('test_file.txt', '~/HOWI3/tmp/libhowi-filesystem/documents');
    
        $this->assertInternalType('string', $this->FS->infoObject('test_file.txt')
            ->getType());
    
        $this->assertEquals('file', $this->FS->infoObject('test_file.txt')
            ->getType());
    }
    
    public function test_isWritable()
    {
    
        $this->FS->infoObject('test_file.txt', '~/HOWI3/tmp/libhowi-filesystem/documents');
    
        $this->assertTrue($this->FS->infoObject('test_file.txt')
            ->isWritable());
    }
    
    public function test_isReadable()
    {
    
        $this->FS->infoObject('test_file.txt', '~/HOWI3/tmp/libhowi-filesystem/documents');
    
        $this->assertTrue($this->FS->infoObject('test_file.txt')
            ->isReadable());
    }
    
    public function test_isExecutable()
    {
    
        $this->FS->infoObject('test_file.txt', '~/HOWI3/tmp/libhowi-filesystem/documents');
    
        $this->assertFalse($this->FS->infoObject('test_file.txt')
            ->isExecutable());
    }
    
    public function test_isFile()
    {
    
        $this->FS->infoObject('test_file.txt', '~/HOWI3/tmp/libhowi-filesystem/documents');
    
        $this->assertTrue($this->FS->infoObject('test_file.txt')
            ->isFile());
    }
    
    public function test_isDir()
    {
    
        $this->FS->infoObject('test_file.txt', '~/HOWI3/tmp/libhowi-filesystem/documents');
    
        $this->assertFalse($this->FS->infoObject('test_file.txt')
            ->isDir(), '~/HOWI3/tmp/libhowi-filesystem/log/phpunitTestLog.txt');
    }
    
    public function test_isLink()
    {
    
        $this->FS->infoObject('test_file.txt', '~/HOWI3/tmp/libhowi-filesystem/documents');
        $this->assertFalse($this->FS->infoObject('test_file.txt')
            ->isLink());
    }
    
    public function test_getRealPath()
    {
    
        $this->FS->infoObject('test_file.txt', '~/HOWI3/tmp/libhowi-filesystem/documents');
    
        $this->assertEquals(getenv('HOME') . '/HOWI3/tmp/libhowi-filesystem/documents/test_file.txt',
            $this->FS->infoObject('test_file.txt')
            ->getRealPath(), 'Failed to assert iterator getPath');
    }
    
    public function test_getFileInfo()
    {
    
        $this->FS->infoObject('test_file.txt', '~/HOWI3/tmp/libhowi-filesystem/documents');
    
        $this->assertInstanceOf('HOWI3\libhowi\Filesystem\Commons\ObjectInterfaces\InfoInterface',
            $this->FS->infoObject('test_file.txt')
            ->getFileInfo());
    }
    
    public function test_getPathInfo()
    {
    
        $this->FS->infoObject('test_file.txt', '~/HOWI3/tmp/libhowi-filesystem/documents');
    
        $this->assertInstanceOf('\HOWI3\libhowi\Filesystem\Commons\ObjectInterfaces\InfoInterface',
            $this->FS->infoObject('test_file.txt')
            ->getPathInfo());
    }
    
    public function test_openFile()
    {
    
        $this->FS->infoObject('test_file.txt', '~/HOWI3/tmp/libhowi-filesystem/documents');
         
        $this->assertInstanceOF('HOWI3\libhowi\Filesystem\Commons\ObjectInterfaces\FileInterface',
            $this->FS->infoObject('test_file.txt')
            ->openFile());
    
        $context = stream_context_create(
            array(
                'socket' => array(
                    'bindto' => '0:7000'
                )
            ));
    
        $this->assertInstanceOF('HOWI3\libhowi\Filesystem\Commons\ObjectInterfaces\FileInterface',
            $this->FS->infoObject('test_file.txt')
            ->openFile('r', false, $context));
    }
    
    public function test_setFileClass()
    {
    
        $this->FS->infoObject('test_file.txt', '~/HOWI3/tmp/libhowi-filesystem/documents');
    
        
        $this->assertTrue(
            $this->FS->infoObject('test_file.txt')
            ->setFileClass('SplFileObject'));
    
        $this->assertFalse(
            $this->FS->infoObject('test_file.txt')
            ->setFileClass('\SplFileObjectNonExisting'));
    
        
        if(PHP_VERSION_ID >= 70000)
        {
            $this->assertTrue(
            $this->FS->infoObject('test_file.txt')
            ->setFileClass('\HOWI3\libhowi\Filesystem\php7\Objects\FileObject'));
        }
        else {
            $this->assertTrue(
            $this->FS->infoObject('test_file.txt')
            ->setFileClass('\HOWI3\libhowi\Filesystem\php5\Objects\FileObject'));
        }
    }
    
    public function test_setInfoClass()
    {
    
        $this->FS->infoObject('test_file.txt', '~/HOWI3/tmp/libhowi-filesystem/documents');
    
        $this->assertTrue(
            $this->FS->infoObject('test_file.txt')
            ->setInfoClass('SplFileInfo'));
    
        $this->assertFalse(
            $this->FS->infoObject('test_file.txt')
            ->setInfoClass('\SplFileInfoNonExisting'));
    
        
        if(PHP_VERSION_ID >= 70000)
        {
           $this->assertTrue(
            $this->FS->infoObject('test_file.txt')
            ->setInfoClass('\HOWI3\libhowi\Filesystem\php7\Objects\infoObject'));
        }
        else {
           $this->assertTrue(
            $this->FS->infoObject('test_file.txt')
            ->setInfoClass('\HOWI3\libhowi\Filesystem\php5\Objects\infoObject'));
        }
    }
    
    public function test___toString()
    {
    
        $this->FS->infoObject('test_file.txt', '~/HOWI3/tmp/libhowi-filesystem/documents');
    
        $expected_first_line = $this->FS->infoObject('test_file.txt')->getRealPath();
        $actual_first_line = $this->FS->infoObject('test_file.txt')->__toString();
        
        $this->assertEquals($expected_first_line, $actual_first_line);
    }
    
    public function test_getOwnerName()
    {
    
        $this->FS->infoObject('test_file.txt', '~/HOWI3/tmp/libhowi-filesystem/documents');
    
        $username = posix_getpwuid(getmyuid());
        $this->assertInternalType('string', $this->FS->infoObject('test_file.txt')
            ->getOwnerName());
        $this->assertEquals($username['name'], $this->FS->infoObject('test_file.txt')
            ->getOwnerName(), 'owners name should match with current users name');
    }
    
    public function test_getGroupName()
    {
    
        $this->FS->infoObject('test_file.txt', '~/HOWI3/tmp/libhowi-filesystem/documents');
    
        $group = posix_getgrgid(getmygid())['name'];
    
        $this->assertEquals($group, $this->FS->infoObject('test_file.txt')
            ->getGroupName(), 'group should match with current users group');
    }
    
    public function test_isDot()
    {
    
        $this->FS->infoObject('test_file.txt', '~/HOWI3/tmp/libhowi-filesystem/documents');
    
        $this->assertFalse($this->FS->infoObject('test_file.txt')->isDot());
    }
    
    public function test_functions()
    {
        $this->FS->infoObject('test_file.txt', '~/HOWI3/tmp/libhowi-filesystem/documents');
        
        $methods = get_class_methods($this->FS->infoObject('test_file.txt'));
        
        foreach ($methods as $method) {
            $skip = array(
                '__construct',
                '_bad_state_ex',
                'ct_ago',
                'isAbsolute',
                'isRelative',
                'makeAbsolute',
                'exists',
                'getCwd'
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