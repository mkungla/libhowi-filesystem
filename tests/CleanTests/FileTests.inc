<?php
use HOWI3\libhowi\Filesystem\FS;


class FileTests extends PHPUnit_Framework_TestCase
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

    public function test_file()
    {
        
        
        $this->assertFalse($this->FS->file());
        $this->assertEquals(601, $this->FS->getCode());
        $this->assertFalse($this->FS->getStatus());
        
        $this->assertFalse($this->FS->file('test_file.txt', '/'));
        $this->assertEquals(503, $this->FS->getCode());
        $this->assertFalse($this->FS->getStatus());
        
        $this->assertFalse($this->FS->file('test_file.txt', '/root'));
        $this->assertEquals(503, $this->FS->getCode());
        $this->assertFalse($this->FS->getStatus());
        
        $this->assertInstanceOf('HOWI3\libhowi\Filesystem\Commons\ObjectInterfaces\FileInterface', 
            $this->FS->file('test_file.txt', '~/HOWI3/tmp/libhowi-filesystem/documents'));
        $this->assertEquals(703, $this->FS->getCode());
        $this->assertFalse($this->FS->getStatus());
    }

    public function test_eof()
    {
        
        $this->FS->file('test_file.txt', '~/HOWI3/tmp/libhowi-filesystem/documents');
        
        $this->assertFalse($this->FS->file('test_file.txt')
            ->eof());
        
        while (! $this->FS->file('test_file.txt')->eof()) {
            $this->FS->file('test_file.txt')->fgets();
        }
        $this->assertTrue($this->FS->file('test_file.txt')
            ->eof());
    }

    public function test_fgetss()
    {
        
        
        $str = <<<EOD
<html><body>
 <p>Welcome! Today is the <?php echo(date('jS')); ?> of <?= date('F'); ?>.</p>
</body></html>
Text outside of the HTML block.
EOD;
        $htmlfile = $this->FS->makeAbsolute('~/HOWI3/tmp/libhowi-filesystem/documents/test.phtml');
        file_put_contents($htmlfile, $str);
        
        $this->FS->file('test.phtml', '~/HOWI3/tmp/libhowi-filesystem/documents');
        
        $data = '';
        while (! $this->FS->file('test.phtml')->eof()) {
            $data .= $this->FS->file('test.phtml')->fgetss();
        }
        
        $expected = "\n" . ' Welcome! Today is the  of .' . "\n\n";
        $expected .= 'Text outside of the HTML block.';
        
        $this->assertEquals($expected, $data);
        
        /* With param */
        $this->FS->file('test.phtml')->rewind();
        $data = '';
        while (! $this->FS->file('test.phtml')->eof()) {
            $data .= $this->FS->file('test.phtml')->fgetss('<p>');
        }
        
        $expected = "\n" . ' <p>Welcome! Today is the  of .</p>' . "\n\n";
        $expected .= 'Text outside of the HTML block.';
        
        $this->assertEquals($expected, $data);
    }

    public function test_csv()
    {
        
        $this->FS->file('test.csv', '~/HOWI3/tmp/libhowi-filesystem/documents');
        
        $csv = [
            [
                'val1',
                'val2',
                'val3'
            ],
            [
                'val1',
                'val2',
                'val3'
            ],
            [
                'val1',
                'val2',
                'val3'
            ]
        ];
        foreach ($csv as $fields) {
            $this->assertInternalType('int', $this->FS->file('test.csv')
                ->fputcsv($fields));
        }
        
        while (! $this->FS->file('test.csv')->eof()) {
            $this->assertInternalType('array', $this->FS->file('test.csv')
                ->fgetcsv());
        }
        
        $this->FS->file('test.csv')->rewind();
        
       
        if(PHP_VERSION_ID >= 70000)
        {
            $this->FS->file('test.csv')->setFlags(\HOWI3\libhowi\Filesystem\php7\Objects\FileObject::READ_CSV);
        }
        else {
            $this->FS->file('test.csv')->setFlags(\HOWI3\libhowi\Filesystem\php5\Objects\FileObject::READ_CSV);
        }
        
        
        foreach ($this->FS->file('test.csv') as $row) {
            if (! is_array($row) || ! array_key_exists(2, $row))
                continue;
            
            list ($col1, $col2, $col3) = $row;
            $stringOfrow = sprintf("1:%s 2:%s 3:%s", $col1, $col2, $col3);
            $this->assertEquals("1:val1 2:val2 3:val3", $stringOfrow);
        }
    }

    public function test_setCsvControl()
    {
        
        $this->FS->file('test.csv', '~/HOWI3/tmp/libhowi-filesystem/documents');
        
        $this->assertNull($this->FS->file('test.csv')
            ->setCsvControl(",", "\"", "\\"));
    }

    public function test_getCsvControl()
    {
        
        $this->FS->file('test.csv', '~/HOWI3/tmp/libhowi-filesystem/documents');
        
        $this->assertInternalType('array', $this->FS->file('test.csv')
            ->getCsvControl());
    }

    public function test_flock()
    {
        
        $this->FS->file('test.csv', '~/HOWI3/tmp/libhowi-filesystem/documents');
        
        $this->assertTrue($this->FS->file('test.csv')
            ->flock(LOCK_EX));
        $this->assertTrue($this->FS->file('test.csv')
            ->flock(LOCK_UN));
        
        $LOCK = 1;
        $this->assertTrue($this->FS->file('test.csv')
            ->flock(LOCK_EX, $LOCK));
       
        $this->assertTrue($this->FS->file('test.csv')
            ->flock(LOCK_UN, $LOCK));
    }

    public function test_fflush()
    {
        
        $this->FS->file('test_file.txt', '~/HOWI3/tmp/libhowi-filesystem/documents');
        
        $this->FS->file('test_file.txt')->fwrite('test_fflush' . "\n");
        
        $this->assertTrue($this->FS->file('test_file.txt')
            ->fflush());
    }

    public function test_ftell()
    {
        
        $this->FS->file('test_file.txt', '~/HOWI3/tmp/libhowi-filesystem/documents');
        $this->assertInternalType('int', $this->FS->file('test_file.txt')
            ->ftell());
    }

    public function test_fgetc()
    {
        
        $this->FS->file('test_file.txt', '~/HOWI3/tmp/libhowi-filesystem/documents');
        $this->assertEquals('t', $this->FS->file('test_file.txt')
            ->fgetc());
    }

    public function test_fpassthru()
    {
        
        $this->FS->file('test_file.txt', '~/HOWI3/tmp/libhowi-filesystem/documents');
        
        $this->FS->file('test_file.txt')->fseek(0, SEEK_END);
        
        $this->assertInternalType('int', $this->FS->file('text')
            ->fpassthru());
    }

    public function test_fscanf()
    {
        
        $this->FS->file('test.csv', '~/HOWI3/tmp/libhowi-filesystem/documents');
        $this->FS->file('test.csv')->rewind();
        
        while ($row = $this->FS->file('test.csv')->fscanf("%s %s %s")) {
            list ($col1, $col2, $col3) = $row;
            $this->assertEquals("val1,val2,val3", $col1 . $col2 . $col3);
        }
    }

    public function test_fwrite()
    {
        
        $this->FS->file('test_file.txt', '~/HOWI3/tmp/libhowi-filesystem/documents');
        
        $this->assertInternalType('int', 
            $this->FS->file('test_file.txt')
                ->fwrite('test_fflush' . "\n", 5));
    }

    public function test_fread()
    {
        
        $this->FS->file('test_file.txt', '~/HOWI3/tmp/libhowi-filesystem/documents');
        
        $contents = $this->FS->file('test_file.txt')->fread(
            $this->FS->file('test_file.txt')
                ->getSize());
        $this->assertInternalType('string', $contents);
    }

    public function test_fstat()
    {
        
        $this->FS->file('test_file.txt', '~/HOWI3/tmp/libhowi-filesystem/documents');
        
        $this->assertInternalType('array', $this->FS->file('test_file.txt')
            ->fstat());
    }

    public function test_ftruncate()
    {
        
        $this->FS->file('test_file.txt', '~/HOWI3/tmp/libhowi-filesystem/documents');
        
        $this->assertTrue($this->FS->file('test_file.txt')
            ->ftruncate(5));
    }

    public function test_getFlags()
    {
        
        $this->FS->file('test_file.txt', '~/HOWI3/tmp/libhowi-filesystem/documents');
        
        $this->assertInternalType('int', $this->FS->file('test_file.txt')
            ->getFlags());
    }

    public function test_setMaxLineLen()
    {
        
        $this->FS->file('test_file.txt', '~/HOWI3/tmp/libhowi-filesystem/documents');
        
        $this->assertNull($this->FS->file('test_file.txt')
            ->setMaxLineLen(50));
    }

    public function test_getMaxLineLen()
    {
        
        $this->FS->file('test_file.txt', '~/HOWI3/tmp/libhowi-filesystem/documents');
        
        $this->assertEquals(0, $this->FS->file('test_file.txt')
            ->getMaxLineLen());
    }

    public function test_hasChildren()
    {
        
        $this->FS->file('test_file.txt', '~/HOWI3/tmp/libhowi-filesystem/documents');
        
        $this->assertFalse($this->FS->file('test_file.txt')
            ->hasChildren());
    }

    public function test_getChildren()
    {
        
        $this->FS->file('test_file.txt', '~/HOWI3/tmp/libhowi-filesystem/documents');
        
        $this->assertFalse($this->FS->file('test_file.txt')
            ->getChildren());
    }

    public function test_seek()
    {
        
        $this->FS->file('test_file.txt', '~/HOWI3/tmp/libhowi-filesystem/documents');
        
        $this->assertNull($this->FS->file('test_file.txt')
            ->seek(2));
    }

    public function test_getCurrentLine()
    {
        
        $this->FS->file('test_file.txt', '~/HOWI3/tmp/libhowi-filesystem/documents');
        
        $expected_first_line = $this->FS->file('test_file.txt')->fgets();
        $this->FS->file('test_file.txt')->rewind();
        
        $actual_first_line = $this->FS->file('test_file.txt')->getCurrentLine();
        $this->assertEquals($expected_first_line, $actual_first_line);
    }

    public function test___toString()
    {
        
        $this->FS->file('test_file.txt', '~/HOWI3/tmp/libhowi-filesystem/documents');
        
        $expected_first_line = $this->FS->file('test_file.txt')->current();
        $this->FS->file('test_file.txt')->rewind();
        
        $actual_first_line = $this->FS->file('test_file.txt')->__toString();
        $this->assertEquals($expected_first_line, $actual_first_line);
    }

    public function test_getPath()
    {
        
        $this->FS->file('test_file.txt', '~/HOWI3/tmp/libhowi-filesystem/documents');
        
        $this->assertEquals(getenv('HOME') . '/HOWI3/tmp/libhowi-filesystem/documents', 
            $this->FS->file('test_file.txt')
                ->getPath(), 'Failed to assert getPath');
    }

    public function test_getFilename()
    {
        
        $this->FS->file('test_file.txt', '~/HOWI3/tmp/libhowi-filesystem/documents');
        
        $this->assertEquals('test_file.txt', $this->FS->file('test_file.txt')
            ->getFilename(), '~/HOWI3/tmp/libhowi-filesystem/log/phpunitTestLog.txt should be string txt');
    }

    public function test_getExtension()
    {
        
        $this->FS->file('test_file.txt', '~/HOWI3/tmp/libhowi-filesystem/documents');
        
        $this->assertEquals('txt', $this->FS->file('test_file.txt')
            ->getExtension(), '~/HOWI3/tmp/libhowi-filesystem/log/test_file.txt should be string txt');
    }

    public function test_getBasename()
    {
        
        $this->FS->file('test_file.txt', '~/HOWI3/tmp/libhowi-filesystem/documents');
        
        $this->assertEquals('test_file.txt', $this->FS->file('test_file.txt')
            ->getBasename());
    }

    public function test_getPathName()
    {
        
        $this->FS->file('test_file.txt', '~/HOWI3/tmp/libhowi-filesystem/documents');
        
        $this->assertEquals(getenv('HOME') . '/HOWI3/tmp/libhowi-filesystem/documents/test_file.txt', 
            $this->FS->file('test_file.txt')
                ->getPathName(), 'Failed to assert getPathName');
    }

    public function test_getPerms()
    {
        
        $this->FS->file('test_file.txt', '~/HOWI3/tmp/libhowi-filesystem/documents');
        
        $this->assertEquals(664, $this->FS->file('test_file.txt')
            ->getPerms(), ' permissions should be 664');
    }

    public function test_getInode()
    {
        
        $this->FS->file('test_file.txt', '~/HOWI3/tmp/libhowi-filesystem/documents');
        
        $this->assertInternalType('int', $this->FS->file('test_file.txt')
            ->getInode());
    }

    public function test_getSize()
    {
        
        $this->FS->file('test_file.txt', '~/HOWI3/tmp/libhowi-filesystem/documents');
        
        $this->assertInternalType('int', $this->FS->file('test_file.txt')
            ->getSize());
        $this->assertInternalType('array', $this->FS->file('test_file.txt')
            ->getSize(true));
    }

    public function test_getOwner()
    {
        
        $this->FS->file('test_file.txt', '~/HOWI3/tmp/libhowi-filesystem/documents');
        
        $userID = getmyuid();
        $this->assertInternalType('int', $this->FS->file('test_file.txt')
            ->getOwner());
        $this->assertEquals($userID, $this->FS->file('test_file.txt')
            ->getOwner(), ' owner should match with current users ID');
    }

    public function test_getGroup()
    {
        
        $this->FS->file('test_file.txt', '~/HOWI3/tmp/libhowi-filesystem/documents');
        
        $group = getmygid();
        
        $this->assertEquals($group, $this->FS->file('test_file.txt')
            ->getGroup(), ' group should match with current users group');
    }

    public function test_getATime()
    {
        
        $this->FS->file('test_file.txt', '~/HOWI3/tmp/libhowi-filesystem/documents');
        
        $this->assertInternalType('int', $this->FS->file('test_file.txt')
            ->getATime(), 'Default getAccessTime must be a int');
        $this->assertInternalType('string', $this->FS->file('test_file.txt')
            ->getATime('ago'), ' getAccessTime must be a string');
        $this->assertInternalType('string', 
            $this->FS->file('test_file.txt')
                ->getATime('ago.single'), ' getAccessTime must be a string');
        $this->assertInternalType('string', $this->FS->file('test_file.txt')
            ->getATime('c'), ' getAccessTime must be a string');
    }

    public function test_getMTime()
    {
        
        $this->FS->file('test_file.txt', '~/HOWI3/tmp/libhowi-filesystem/documents');
        
        $this->assertInternalType('int', $this->FS->file('test_file.txt')
            ->getMTime(), 'Default getModifTime must be a int');
        $this->assertInternalType('string', $this->FS->file('test_file.txt')
            ->getMTime('ago'), ' getModifTime must be a string');
        $this->assertInternalType('string', 
            $this->FS->file('test_file.txt')
                ->getMTime('ago.single'), ' getModifTime must be a string');
        $this->assertInternalType('string', $this->FS->file('test_file.txt')
            ->getMTime('c'), ' getModifTime must be a string');
    }

    public function test_getCTime()
    {
        
        $this->FS->file('test_file.txt', '~/HOWI3/tmp/libhowi-filesystem/documents');
        
        $this->assertInternalType('int', $this->FS->file('test_file.txt')
            ->getCTime(), 'Default getAccessTime must be a int');
        $this->assertInternalType('string', $this->FS->file('test_file.txt')
            ->getCTime('ago'), ' getAccessTime must be a string');
        $this->assertInternalType('string', 
            $this->FS->file('test_file.txt')
                ->getCTime('ago.single'), ' getAccessTime must be a string');
        $this->assertInternalType('string', $this->FS->file('test_file.txt')
            ->getCTime('c'), ' getAccessTime must be a string');
    }

    public function test_getType()
    {
        
        $this->FS->file('test_file.txt', '~/HOWI3/tmp/libhowi-filesystem/documents');
        
        $this->assertInternalType('string', $this->FS->file('test_file.txt')
            ->getType());
        
        $this->assertEquals('file', $this->FS->file('test_file.txt')
            ->getType());
    }

    public function test_isWritable()
    {
        
        $this->FS->file('test_file.txt', '~/HOWI3/tmp/libhowi-filesystem/documents');
        
        $this->assertTrue($this->FS->file('test_file.txt')
            ->isWritable());
    }

    public function test_isReadable()
    {
        
        $this->FS->file('test_file.txt', '~/HOWI3/tmp/libhowi-filesystem/documents');
        
        $this->assertTrue($this->FS->file('test_file.txt')
            ->isReadable());
    }

    public function test_isExecutable()
    {
        
        $this->FS->file('test_file.txt', '~/HOWI3/tmp/libhowi-filesystem/documents');
        
        $this->assertFalse($this->FS->file('test_file.txt')
            ->isExecutable());
    }

    public function test_isFile()
    {
        
        $this->FS->file('test_file.txt', '~/HOWI3/tmp/libhowi-filesystem/documents');
        
        $this->assertTrue($this->FS->file('test_file.txt')
            ->isFile());
    }

    public function test_isDir()
    {
        
        $this->FS->file('test_file.txt', '~/HOWI3/tmp/libhowi-filesystem/documents');
        
        $this->assertFalse($this->FS->file('test_file.txt')
            ->isDir(), '~/HOWI3/tmp/libhowi-filesystem/log/phpunitTestLog.txt');
    }

    public function test_isLink()
    {
        
        $this->FS->file('test_file.txt', '~/HOWI3/tmp/libhowi-filesystem/documents');
        $this->assertFalse($this->FS->file('test_file.txt')
            ->isLink());
    }

    public function test_getRealPath()
    {
        
        $this->FS->file('test_file.txt', '~/HOWI3/tmp/libhowi-filesystem/documents');
    
        $this->assertEquals(getenv('HOME') . '/HOWI3/tmp/libhowi-filesystem/documents/test_file.txt',
            $this->FS->file('test_file.txt')
            ->getRealPath(), 'Failed to assert iterator getPath');
    }
    
    public function test_getFileInfo()
    {
        
        $this->FS->file('test_file.txt', '~/HOWI3/tmp/libhowi-filesystem/documents');
    
        $this->assertInstanceOf('HOWI3\libhowi\Filesystem\Commons\ObjectInterfaces\InfoInterface',
            $this->FS->file('test_file.txt')
            ->getFileInfo());
    }
    
    public function test_getPathInfo()
    {
        
        $this->FS->file('test_file.txt', '~/HOWI3/tmp/libhowi-filesystem/documents');
    
        $this->assertInstanceOf('\HOWI3\libhowi\Filesystem\Commons\ObjectInterfaces\InfoInterface',
            $this->FS->file('test_file.txt')
            ->getPathInfo());
    }
    
    public function test_openFile()
    {
        
        $this->FS->file('test_file.txt', '~/HOWI3/tmp/libhowi-filesystem/documents');
         
        $this->assertInstanceOF('HOWI3\libhowi\Filesystem\Commons\ObjectInterfaces\FileInterface',
            $this->FS->file('test_file.txt')
            ->openFile());
    
        $context = stream_context_create(
            array(
                'socket' => array(
                    'bindto' => '0:7000'
                )
            ));
    
        $this->assertInstanceOF('HOWI3\libhowi\Filesystem\Commons\ObjectInterfaces\FileInterface',
            $this->FS->file('test_file.txt')
            ->openFile('r', false, $context));
    }
    
    public function test_setFileClass()
    {
        
        $this->FS->file('test_file.txt', '~/HOWI3/tmp/libhowi-filesystem/documents');
        
        $this->assertTrue(
            $this->FS->file('test_file.txt')
            ->setFileClass('SplFileObject'));
    
        $this->assertFalse(
            $this->FS->file('test_file.txt')
            ->setFileClass('\SplFileObjectNonExisting'));
    
        if(PHP_VERSION_ID >= 70000)
        {
           $this->assertTrue(
            $this->FS->file('test_file.txt')
            ->setFileClass('\HOWI3\libhowi\Filesystem\php7\Objects\FileObject'));
        }
        else {
           $this->assertTrue(
            $this->FS->file('test_file.txt')
            ->setFileClass('\HOWI3\libhowi\Filesystem\php5\Objects\FileObject'));
        }
        
    }
    
    public function test_setInfoClass()
    {
        
        $this->FS->file('test_file.txt', '~/HOWI3/tmp/libhowi-filesystem/documents');
        
        $this->assertTrue(
            $this->FS->file('test_file.txt')
            ->setInfoClass('SplFileInfo'));
    
        $this->assertFalse(
            $this->FS->file('test_file.txt')
            ->setInfoClass('\SplFileInfoNonExisting'));
    
        
        
        if(PHP_VERSION_ID >= 70000)
        {
            $this->assertTrue(
            $this->FS->file('test_file.txt')
            ->setInfoClass('\HOWI3\libhowi\Filesystem\php7\Objects\infoObject'));
        }
        else {
            $this->assertTrue(
            $this->FS->file('test_file.txt')
            ->setInfoClass('\HOWI3\libhowi\Filesystem\php5\Objects\infoObject'));
        }
    }
    
    public function test_getOwnerName()
    {
        
        $this->FS->file('test_file.txt', '~/HOWI3/tmp/libhowi-filesystem/documents');
        
        $username = posix_getpwuid(getmyuid());
        $this->assertInternalType('string', $this->FS->file('test_file.txt')
            ->getOwnerName());
        $this->assertEquals($username['name'], $this->FS->file('test_file.txt')
            ->getOwnerName(), 'owners name should match with current users name');
    }
    
    public function test_getGroupName()
    {
        
        $this->FS->file('test_file.txt', '~/HOWI3/tmp/libhowi-filesystem/documents');
        
        $group = posix_getgrgid(getmygid())['name'];
    
        $this->assertEquals($group, $this->FS->file('test_file.txt')
            ->getGroupName(), 'group should match with current users group');
    }
    
    public function test_isDot()
    {
        
        $this->FS->file('test_file.txt', '~/HOWI3/tmp/libhowi-filesystem/documents');
        
        $this->assertFalse($this->FS->file('test_file.txt')->isDot());
    }
    
    public function test_key()
    {
        
        $this->FS->file('test_file.txt', '~/HOWI3/tmp/libhowi-filesystem/documents');
        
        $this->assertInternalType('int', $this->FS->file('test_file.txt')
            ->key());
    }
    
    public function test_current()
    {
        
        $this->FS->file('test_file.txt', '~/HOWI3/tmp/libhowi-filesystem/documents');
        
        $this->assertInternalType('int',
            $this->FS->file('test_file.txt')
            ->fwrite('HOWI3 Filesystem phpunit tests' . "\n"));
        
        $this->FS->file('test_file.txt')->rewind();
        
        $this->assertInternalType('string', $this->FS->file('test_file.txt')
            ->current());
        $this->assertEquals('HOWI3 Filesystem phpunit tests' . "\n",
            $this->FS->file('test_file.txt')
            ->current());
    }
    
    public function test_getLinkTarget()
    {
        
        $this->FS->file('test_file.txt', '~/HOWI3/tmp/libhowi-filesystem/documents');
        
        $this->assertFalse($this->FS->file('test_file.txt')
            ->getLinkTarget());
    }
    
    public function test_dummyScale()
    {
        $this->FS->file('big_dummy.mov','~/HOWI3/tmp/libhowi-filesystem/videos');
        $this->assertTrue($this->FS->file('big_dummy.mov')->dummyScale(25000000));
    }
    public function test_functions()
    {
        
        $this->FS->file('test_file.txt', '~/HOWI3/tmp/libhowi-filesystem/documents');
        
        $methods = get_class_methods($this->FS->file('test_file.txt'));
        
        foreach ($methods as $method) {
            $skip = array(
                '__construct',
                'rewind',
                'valid',
                'fgets',
                'fgetcsv',
                'fputcsv',
                'fseek',
                'next',
                'setFlags',
                '_bad_state_ex',
                'getCwd',
                'makeAbsolute',
                'exists',
                'rm',
                'isAbsolute',
                'isRelative',
                'ct_ago',
                'cd'
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