--------------------------------------------------------------------------------
-- Company: 
-- Engineer:
--
-- Create Date:   08:51:44 03/28/2017
-- Design Name:   
-- Module Name:   /home/anders/Dropbox/Robtek/4_semester/DIG/Projekt/EncoderBench.vhd
-- Project Name:  Projekt
-- Target Device:  
-- Tool versions:  
-- Description:   
-- 
-- VHDL Test Bench Created by ISE for module: Encoder
-- 
-- Dependencies:
-- 
-- Revision:
-- Revision 0.01 - File Created
-- Additional Comments:
--
-- Notes: 
-- This testbench has been automatically generated using types std_logic and
-- std_logic_vector for the ports of the unit under test.  Xilinx recommends
-- that these types always be used for the top-level I/O of a design in order
-- to guarantee that the testbench will bind correctly to the post-implementation 
-- simulation model.
--------------------------------------------------------------------------------
LIBRARY ieee;
USE ieee.std_logic_1164.ALL;
 
-- Uncomment the following library declaration if using
-- arithmetic functions with Signed or Unsigned values
--USE ieee.numeric_std.ALL;
 
ENTITY EncoderBench IS
END EncoderBench;
 
ARCHITECTURE behavior OF EncoderBench IS 
 
    -- Component Declaration for the Unit Under Test (UUT)
 
    COMPONENT Encoder
    PORT(
         Reset : IN  std_logic;
         CLK : IN  std_logic;
         HallSensor : IN  std_logic_vector(1 downto 0);
         Led : OUT  std_logic_vector(7 downto 0)
        );
    END COMPONENT;
    

   --Inputs
   signal Reset : std_logic := '0';
   signal CLK : std_logic := '0';
   signal HallSensor : std_logic_vector(1 downto 0) := (others => '0');

 	--Outputs
   signal Led : std_logic_vector(7 downto 0);

   -- Clock period definitions
   constant CLK_period : time := 10 ns;
 
BEGIN
 
	-- Instantiate the Unit Under Test (UUT)
   uut: Encoder PORT MAP (
          Reset => Reset,
          CLK => CLK,
          HallSensor => HallSensor,
          Led => Led
        );

   -- Clock process definitions
   CLK_process :process
   begin
		CLK <= '0';
		wait for CLK_period/2;
		CLK <= '1';
		wait for CLK_period/2;
   end process;
 

   -- Stimulus process
   stim_proc: process
   begin		
      -- hold reset state for 100 ns.
      wait for 100 ns;	

      wait for CLK_period*10;

      -- insert stimulus here 

      wait;
   end process;

END;
