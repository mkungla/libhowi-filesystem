<?php
use HOWI3\libhowi\Filesystem\FS;

class DirectoryTests extends PHPUnit_Framework_TestCase
{

    protected $FS;

    public function setUp()
    {
        if($this->FS instanceof HOWI3\libhowi\Filesystem\FS)
            return;
        
        $this->FS = FS::load();
        $logfile = $this->FS->makeAbsolute(FS_TEST_LOG_LOCATION);
        $this->FS->setLogLevel(FS_TEST_LOG_LEVEL);
        $this->FS->setUID(getmyuid());
        $this->FS->setUsername(get_current_user());
        $this->FS->setLogFile($logfile);
    }

    public function test_make_new_directories()
    {
        
        
        /* SUCCESS */
        
        /* First parameter $keyword is also the directory basename */
        /* make new dir */
        $this->assertInstanceOf(
            '\HOWI3\libhowi\Filesystem\Commons\ObjectInterfaces\DirectoryTreeInterface', 
            $this->FS->dir('videos', FS_TEST_LOCATION));
        $this->assertTrue($this->FS->getStatus());
        $this->assertEquals(701, $this->FS->getCode());
        
        /* make new dir also recursive since images does not exist */
        
        $this->assertInstanceOf(
            '\HOWI3\libhowi\Filesystem\Commons\ObjectInterfaces\DirectoryTreeInterface',
            $this->FS->dir('photos', '~/HOWI3/tmp/libhowi-filesystem/images'));
        $this->assertTrue($this->FS->getStatus());
        $this->assertEquals(701, $this->FS->getCode());
        
        /* Now images do exist */
        
        $this->assertInstanceOf(
            '\HOWI3\libhowi\Filesystem\Commons\ObjectInterfaces\DirectoryTreeInterface',
            $this->FS->dir('png', '~/HOWI3/tmp/libhowi-filesystem/images'));
        $this->assertTrue($this->FS->getStatus());
        $this->assertEquals(701, $this->FS->getCode());
        
        
        $this->assertInstanceOf(
            '\HOWI3\libhowi\Filesystem\Commons\ObjectInterfaces\DirectoryTreeInterface',
            $this->FS->dir('gif', '~/HOWI3/tmp/libhowi-filesystem/images'));
        $this->assertTrue($this->FS->getStatus());
        $this->assertEquals(701, $this->FS->getCode());
        
        
        $this->assertInstanceOf(
            '\HOWI3\libhowi\Filesystem\Commons\ObjectInterfaces\DirectoryTreeInterface',
            $this->FS->dir('jpg', '~/HOWI3/tmp/libhowi-filesystem/images'));
        $this->assertTrue($this->FS->getStatus());
        $this->assertEquals(701, $this->FS->getCode());
        
        
        $this->assertInstanceOf(
            '\HOWI3\libhowi\Filesystem\Commons\ObjectInterfaces\DirectoryTreeInterface',
            $this->FS->dir('raw', '~/HOWI3/tmp/libhowi-filesystem/images'));
        $this->assertTrue($this->FS->getStatus());
        $this->assertEquals(701, $this->FS->getCode());
        
        /* Create directory with other than default permissions which are 750 */
        
        $this->assertInstanceOf(
            '\HOWI3\libhowi\Filesystem\Commons\ObjectInterfaces\DirectoryTreeInterface',
            $this->FS->dir('archives', FS_TEST_LOCATION, true, 755));
        $this->assertTrue($this->FS->getStatus());
        $this->assertEquals(701, $this->FS->getCode());
        
        /* Create directory with context (dummy context) */
        $context = stream_context_create(
            array(
                'socket' => array(
                    'bindto' => '0:7000'
                )
            ));
        
        $this->assertInstanceOf(
            '\HOWI3\libhowi\Filesystem\Commons\ObjectInterfaces\DirectoryTreeInterface',
            $this->FS->dir('documents', FS_TEST_LOCATION, false, 750, $context));
        $this->assertTrue($this->FS->getStatus());
        $this->assertEquals(701, $this->FS->getCode());
        
        /* FAILURES */
        /* If you wan't to prevent directory creation to be recursive */
        
        $this->assertInstanceOf(
            '\HOWI3\libhowi\Filesystem\Commons\ObjectInterfaces\DirectoryPlaceholderInterface',
            $this->FS->dir('pdf', '~/HOWI3/tmp/libhowi-filesystem/doc', false));
        $this->assertFalse($this->FS->getStatus());
        $this->assertEquals(502, $this->FS->getCode());
        
        /* You dry to create directory to location you don't have permissions */
        
        $this->assertInstanceOf(
            '\HOWI3\libhowi\Filesystem\Commons\ObjectInterfaces\DirectoryPlaceholderInterface',
            $this->FS->dir('my_root', '/root'));
        $this->assertFalse($this->FS->getStatus());
        $this->assertEquals(502, $this->FS->getCode());
    }
    
    public function test_DirectoryPlaceholderObject()
    {
        
        
        $this->assertInstanceOf(
            '\HOWI3\libhowi\Filesystem\Commons\ObjectInterfaces\DirectoryPlaceholderInterface',
            $this->FS->dir('my_root', '/root'));
        $this->assertInstanceOf(
            '\HOWI3\libhowi\Filesystem\Commons\ObjectInterfaces\DirectoryPlaceholderInterface',
            $this->FS->dir('my_root'));
        
        $this->FS->dir('my_root')->dirsetPath();
        $this->assertFalse($this->FS->getStatus());
        $this->assertEquals(502, $this->FS->getCode());
        
        $this->FS->dir('my_root')->dirsetMode();
        $this->assertFalse($this->FS->getStatus());
        $this->assertEquals(802, $this->FS->getCode());
        
        
        $this->FS->dir('my_root')->dirsetRecursive();
        $this->assertFalse($this->FS->getStatus());
        $this->assertEquals(802, $this->FS->getCode());
        
        $this->FS->dir('my_root')->dirsetContext();
        $this->assertFalse($this->FS->getStatus());
        $this->assertEquals(802, $this->FS->getCode());
        
        $this->FS->dir('my_root')->dirCreate();
        $this->assertFalse($this->FS->getStatus());
        $this->assertEquals(802, $this->FS->getCode());
        
        $this->FS->dir('my_root')->getPath();
        $this->assertFalse($this->FS->getStatus());
        $this->assertEquals(502, $this->FS->getCode());
    }
    public function test_get_existing_dir()
    {
        
        $this->assertInstanceOf(
            '\HOWI3\libhowi\Filesystem\Commons\ObjectInterfaces\DirectoryTreeInterface',
            $this->FS->dir('documents', FS_TEST_LOCATION));
        
        $this->assertTrue($this->FS->getStatus());
        $this->assertEquals(807, $this->FS->getCode());
        
        /* Code has not been changed since we just call exiting object */
        $this->FS->dir('documents');
        $this->assertEquals(0, $this->FS->getCode());
    }
    
    public function test_load_cwd()
    {
        
        $this->assertIFalse(
            '\HOWI3\libhowi\Filesystem\Commons\ObjectInterfaces\DirectoryTreeInterface',
            $this->FS->dir());
        $this->assertTrue($this->FS->getStatus());
        $this->assertEquals(801, $this->FS->getCode());
    }
    protected function tearDown()
    {
        unset($this->FS);
    }
    
}