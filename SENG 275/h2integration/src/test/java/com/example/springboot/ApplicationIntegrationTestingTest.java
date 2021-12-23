package com.example.springboot;

import com.example.springboot.data.StudentRepository;
import com.example.springboot.domain.Student;
import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.Test;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.boot.test.context.SpringBootTest;
import org.springframework.test.annotation.DirtiesContext;

import java.util.List;

@SpringBootTest(classes = Application.class)
@DirtiesContext(classMode = DirtiesContext.ClassMode.BEFORE_EACH_TEST_METHOD)
public class ApplicationIntegrationTestingTest {

    @Autowired
    private StudentRepository studentRepository;

    @BeforeEach
    public void setUp(){
        // Get a clean database each time so test's are isolated
        studentRepository.deleteAll();
    }

    // Existing example
    @Test
    public void getUser(){
        System.out.println("Number of students are "+studentRepository.findAll());
        Student student = new Student("Sanjay", "Dutt",0);
        Student student1 = new Student("A", "B",1);
        studentRepository.save(student);
        studentRepository.save(student1);
        System.out.println("Number of students are "+studentRepository.findAll());
        System.out.println(studentRepository.findAllActiveStudents());
        System.out.println(studentRepository.findAllStudentsSortedByFirstName());
    }

    // Ex 1
    @Test
    public void SaveTest(){
        // Check no tuples exist in DB
        int initialCount = (int)studentRepository.count();
        assert(initialCount == 0);
        // Save tuple in DB
        Student student = new Student("Amaan", "Makhani", 1);
        studentRepository.save(student);
        // Check that count increases by 1 and new tuple is within DB
        List<Student> temp = (List<Student>)studentRepository.findAll();
        assert(initialCount + 1 == (int)studentRepository.count());
        assert(temp.get(0).getFirstName() == "Amaan" && temp.get(0).getLastName() == "Makhani" && temp.get(0).getActiveStatus() == 1);
    }

    // Ex 2
    @Test
    public void findActiveStudentsTest(){
        // Insert multiple tuples and check that they exists in DB
        int initialCount = (int)studentRepository.count();
        assert(initialCount == 0);
        Student student_1 = new Student("A", "E", 1);
        Student student_2 = new Student("B", "F", 0);
        Student student_3 = new Student("C", "G", 1);
        Student student_4 = new Student("D", "H", 0);
        studentRepository.save(student_1);
        studentRepository.save(student_2);
        studentRepository.save(student_3);
        studentRepository.save(student_4);
        assert(initialCount + 4 == (int)studentRepository.count());
        List<Student> temp = studentRepository.findAllActiveStudents();
        // Check the correct tuples are returned
        assert(temp.get(0).getFirstName() == "A" && temp.get(0).getLastName() == "E" && temp.get(0).getActiveStatus() == 1);
        assert(temp.get(1).getFirstName() == "C" && temp.get(1).getLastName() == "G" && temp.get(1).getActiveStatus() == 1);
    }

    // Ex 3
    @Test
    public void getAllStudentsOrderByFirstName(){
        // Insert multiple tuples and check that they exists in DB
        int initialCount = (int)studentRepository.count();
        assert(initialCount == 0);
        Student student_4 = new Student("D", "H", 0);
        Student student_3 = new Student("C", "G", 1);
        Student student_1 = new Student("A", "E", 1);
        Student student_2 = new Student("B", "F", 0);
        studentRepository.save(student_1);
        studentRepository.save(student_2);
        studentRepository.save(student_3);
        studentRepository.save(student_4);
        assert(initialCount + 4 == (int)studentRepository.count());
        List<Student> temp = studentRepository.findAllStudentsSortedByFirstName();
        assert(temp.get(0).getFirstName() == "A");
        assert(temp.get(1).getFirstName() == "B");
        assert(temp.get(2).getFirstName() == "C");
        assert(temp.get(3).getFirstName() == "D");
    }
}
