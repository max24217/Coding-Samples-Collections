

import java.util.*;
import java.io.*;


public class Assig5
{


	public static void main(String args[])
	{

		char[] c;//used to store the letters
		int[] kell = new int[1];//used to count through the array of char
		kell[0] = 0;
		int[] k = new int[1];//used to count through array of string
		k[0] = 0;
		StringBuilder[] code;//used to store string version of each code for each letter
		Scanner reader = null;//used to read in input from file and create a binary tree
		Scanner reader3 = null;//used to count how many space do we need to create for the arrays


		if (args.length == 0)//user did not input any file name
		{
			System.err.println("\nPlease enter a vaild file name.\n");
			System.exit(1);//no file input, terminate the program
		}

		try//initilizing the reader
		{
			reader = new Scanner(new File(args[0]));
			reader3 = new Scanner(new File(args[0]));
		}
		catch (FileNotFoundException e) 
		{
			System.err.println(e.getMessage());
		}

		int count = 0;//used to count how many leaves 
		//first scann to determine how large our array should be
		while (reader3.hasNextLine())
		{
			String temp = reader3.nextLine();

			if (temp.charAt(0) == 'L')//we find a leaf
			{
				count ++;
			}
		}

		//special case where there is no characters in the file
		if (count == 0)
		{
			throw new SecurityException("There is no letters in the file.\n");
		}

		c = new char[count];
		code = new StringBuilder[count];


		BinaryNode<Character> root = null;
		

		root = buildTree(root,reader);//create a new binary tree and link it to the root node

		StringBuilder out = new StringBuilder();//used to contain the path of certain character
		table(c,kell,code,k,root,out);//used to gather information for creating a table


		//-----------------------------------------------------------------------------------------------------------------


		System.out.println("\nThe Huffman Tree has been restored.");//finish intializing the coding table 

		//starting to run the main process of the programm
		while(true)//will never stop iteration until user chooses quite
		{
			Scanner reader2 = null;
			try//initilizing the scanner to read input 
			{
				reader2 = new Scanner(System.in);
			}
			catch (Exception e) 
			{
				System.err.println(e.getMessage());
			}
		
			System.out.println("\nHere is the coding table: ");
			//print out the table
			for (int i = 0; i < count; i ++)
			{
				System.out.println(c[i] + ": " + code[i].toString());
			}

			System.out.println();
			System.out.println("Please choose from the following: \n");
			System.out.println("1) Encode a text string");
			System.out.println("2) Decode a Huffman string");
			System.out.println("3) Quit");
			int input = reader2.nextInt();
			String inputWord = null;//used to catch input string

			if (input == 3)//user choose to quit the program
			{
				System.out.println("\nThanks for using the programm and have a nice day. >_<");
				System.exit(0);
			}//end of quit

			else if (input == 2)//user choose to Decode a string
			{
				System.out.println("Please enter a string of binary numbers (One line, no space): ");
				inputWord = reader2.next();
				int katsuma = inputWord.length();
				StringBuilder phtanmu = new StringBuilder();//output string
				BinaryNode<Character> pointer = root;//a pointer traverse through the tree

				//start to traverse through the tree and finding correct letters
				for (int i = 0; i < katsuma; i ++)
				{
					if (pointer.isLeaf() == false)// we did not reach leaf yet
					{
						if (inputWord.charAt(i) == '0')//we should go left
						{
							pointer = pointer.getLeftChild();
							if (pointer.isLeaf() == true)//find letter
							{
								phtanmu = phtanmu.append(pointer.getData());
								pointer = root;
							}
							continue;
						}
						else if (inputWord.charAt(i) == '1')//we should go right
						{
							pointer = pointer.getRightChild();
							if (pointer.isLeaf() == true)//find letter
							{
								phtanmu = phtanmu.append(pointer.getData());
								pointer = root;
							}
							continue;
						}
						else if (inputWord.charAt(i) != '1' && inputWord.charAt(i) != '0')//invaild number
						{
							System.err.println("The number is invaild in decoding section.");
							System.exit(1);//terminate the program beacause of an error
						}
					}
				}

				if (pointer.equals(root) == false)//normally the pointer should always point to root after a complete decoding process
				{
					System.err.println("The number is invaild in decoding section.");
					System.exit(1);//terminate the program beacause of an error
				}

				//print out the string
				System.out.println();
				System.out.println("Text String is: " + phtanmu.toString());

			}//end of decoding

			else if (input == 1)//user chooses to encode a string
			{
				System.out.println("\nPlease enter a string from following characters: (all in upper case)");
				
				//print out all possible characters
				for (int i = 0; i < count; i++)
				{
					System.out.print(c[i]);
				}
				System.out.println();


				inputWord = reader2.next();
				int katsuma = inputWord.length();//input string's length
				int nine = 0;//used to check if there is a character is not within range

				//to check if the input is vaild
				for (int i = 0; i < katsuma; i ++)
				{
					char kp = inputWord.charAt(i);
					nine = 0;
					for (int h = 0; h < count; h ++)
					{
						if (kp != c[h])
						{
							nine ++;
							if (nine == count)//we iterate all the possible characters and find nothin
							{
								System.err.println("Please input a vaild input in the encoding section.\n");
								System.exit(1);//terminate program because of an error
							}
						}
						else if (kp == c[h])
						{
							break;
						}
					}
				}	

				
				StringBuilder phtanmu = new StringBuilder();//output string 

				//start to encode the string
				for (int i = 0; i < katsuma; i ++)
				{
					char kp = inputWord.charAt(i);
					for (int w = 0; w < count; w ++)
					{
						if (kp == c[w])//we find the character in the table
						{
							phtanmu = phtanmu.append(code[w].toString());
							break;
						}
					}
				}

				System.out.println();
				System.out.println(phtanmu.toString());//print out the encoding

			}//end of encoding

			else if (input != 1 && input != 2 && input != 3)//user does not choose any vaild option
			{
				System.out.println("Please select a vaild choice.\n");
				continue;
			}

		}//end of while(true)
		


	}//end of main method

	//a recursive method used to create a tree base on a given root
	public static BinaryNode<Character> buildTree(BinaryNode<Character> node, Scanner reader)
	{
		if (reader.hasNextLine()  == false)//there is no more lines in the file to read in
		{
			return node;
		}
		else//there is more to process
		{
			String u12 = reader.nextLine();
			if (u12.charAt(0) == 'I')//it is an interior node
			{
				
				node = new BinaryNode<Character>('\0');
				BinaryNode<Character> newNode = null;
				//recursively call build tree
				//direction is what child the current node is to the previous node
				//0 is left side and 1 is right side
				//if direction is 3, it is root
				node.setLeftChild(buildTree(newNode,reader));
				node.setRightChild(buildTree(newNode,reader));

				return node;
			}
			else if (u12.charAt(0) == 'L')//it is a leaf
			{
				char mikato = u12.charAt(2);//get the letter
				node = new BinaryNode<Character>(mikato);
				return node;

			}
			else//error case, should never reach
			{
				System.err.println("We get to the error case of the buildTree");
			}
		}

		System.err.println("Bogus on the build tree");
		return node;//should never reach here since if else statement should cover every condition
	}//end of recursive binary tree

	//method used to find the encoding table for each letter
	public static void table(char[] c,int[] i,StringBuilder[] code,int[] k,BinaryNode<Character> node,StringBuilder out)
	{
		if (node.isLeaf() == false)//we do not reach leaf yet
		{
			out = out.append("0");//append a zero to look at left first
			table(c,i,code,k,node.getLeftChild(),out);
			out = out.deleteCharAt(out.length()-1);//back track to the node and delete the last number in the string
			out = out.append("1");//append a 1 to look at right 
			table(c,i,code,k,node.getRightChild(),out);
			out = out.deleteCharAt(out.length()-1);//back track, delete the last number
		}
		else if (node.isLeaf() == true)//we get to the leaf
		{
			c[i[0]++] = node.getData();
			code[k[0]++] = new StringBuilder(out.toString());
		}
	}//end of table method

}//end of Assig5