/* Majordome's version
 *
 * Copyright 2018-2025 Laurent Faillie
 *
 * 		Majordome is covered by
 *		Creative Commons Attribution-NonCommercial 3.0 License
 *      (http://creativecommons.org/licenses/by-nc/3.0/) 
 *      Consequently, you're free to use if for personal or non-profit usage,
 *      professional or commercial usage REQUIRES a commercial licence.
 *  
 *      Majordome is distributed in the hope that it will be useful,
 *      but WITHOUT ANY WARRANTY; without even the implied warranty of
 *      MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 *
 *	05/07/2018 - LF - Start of development
 *	25/07/2018 - LF - Switch to C++ (when it's useful)
 *	02/10/2018 - LF - Start logging before subscribing in order to get early
 *		messages as well
 *	16/03/2019 - LF - Externalize Version
 *	04/08/2020 - v3.08 - LF - Add %ClientID% in topic parsing
 *	05/08/2020 - v3.09 - LF - Amost all 'I' migrated to 'T' to be less noisy
 *	29/08/2020 - v3.10 - LF - Handle tracker's function return code
 *	29/08/2020 - v3.11 - LF - Add MAJORDOME_TOPIC_NAME Lua's variable
 *	04/12/2020 - v3.12 - LF - Trace message can be silenced
 *	09/12/2020 - v3.13 - LF - add getName()
 *		---
 *	12/05/2024 - v4.00 - LF - Migrate to SeleneV7
 *	26/10/2024 - V4.01 - LF - Add MinMax
 *	27/10/2024 - V4.02 - LF - Add NamedMinMax
 *	29/10/2024 - V4.03 - LF - NamedMinMax's Lua can return a value
 *		---
 *	24/11/2024 - v5.00 - LF - Introduce Toile
 *		It was only a PoC with bad programming habits.
 *	20/01/2025 - v6.00 - LF - Code redesign
 *	20/02/2025 - v6.01 - LF - Add PostgreSQL support
 *	26/03/2025 - V6.02 - LF - Redesign configuration parsing
 *	01/05/2025 - V6.03 - LF - Generate D2 file
 *	22/05/2025 - V6.04 - LF - Add group
 *	22/05/2025 - V6.05 - LF - Add local verbosity
 *	29/05/2025 - v6.06 - LF - Add AggregatedFeed's preprocessing function
 *	02/06/2025 - v6.07 - LF - Add MMA to AggregatedFeed
 *	10/06/2025 - v6.08 - LF - Add MultiKeyMinMax
 *	12/06/2025 - v6.09 - LF - Add field in archiving
 *	15/06/2025 - v6.10 - LF - Add Archiving's MMA2
 *	15/06/2025 - v6.11 - LF - Expose MajordomeMQTTTopic.match()
 */

#define VERSION 6.1100
#define COPYRIGHT "Majordome (c) L.Faillie 2018-2025"
