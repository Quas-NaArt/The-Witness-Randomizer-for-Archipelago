#include "Hints.h"

const std::string& GetCreditsHint() {
  static const std::string creditsHint = "This Randomizer is brought to you by\n"
    "NewSoupVi, Jarno, blastron,\n"
    "jbzdarkid, sigma144, IHNN, oddGarrett, Exempt-Medic, hatkirby.";
  return creditsHint;
}

const std::vector<std::string>& GetJokeHints() {
  static const std::vector<std::string> jokeHints = {
    "Have you tried Adventure?\n...Holy crud, that game is 17 years older than me.",
    reinterpret_cast<const char*>(u8"Have you tried Aquaria?\nAdmittedly, singing is a much cooler interaction mechanic than \"drawing lines on stuff\"™."),
    "Have you tried A Hat in Time?\nThere's an actual metro in that game, not just one on the tracker.",
    "Have you tried A Link to the Past?\nThe Archipelago game that started it all!",
    "Waiting to get your items?\nTry APSudoku! Make progress even while stuck.",
    "Have you tried Blasphemous?\nYou haven't? Blasphemy!\n...Sorry. You should try it, though!",
    "Have you tried Bomb Rush Cyberfunk?\nYou may try to strike it down, but the spirit of Jet Set Radio lives on.",
    "Have you tried Bumper Stickers?\nDecades after its inception, people are still inventing unique twists on the match-3 genre.",
    "Have you tried Bumper Stickers?\nMaybe after spending so much time on this island, you are longing for a simpler puzzle game.",
    "Have you tried Celeste 64?\nYou need smol low-poly Madeline in your life. TRUST ME.",
    "Have you tried ChecksFinder?\nIf you like puzzles, you might enjoy it!",
    "Have you tried Clique?\nIt's certainly a lot less complicated than this game!",
    "Have you tried Castlevania 64?\nMaybe you can get Malus to play \"In the Hall of the Mountain King\".",
    "Have you tried Dark Souls III?\nA tough game like this feels better when friends are helping you!",
    "Have you tried Donkey Kong Country 3?\nA legendary game from a golden age of platformers!",
    "Have you tried DLC Quest?\nI know you all like parody games.\nI got way too many requests to make a randomizer for \"The Looker\".",
    "Have you tried Doom?\nI wonder if a smart fridge can connect to Archipelago.",
    "Have you tried Doom II?\nGot a good game on your hands? Just make it bigger and better.",
    "Have you tried Factorio?\nAlone in an unknown multiworld. Sound familiar?",
    "Have you tried Final Fantasy?\nExperience a classic game improved to fit modern standards!",
    "Have you tried Final Fantasy Mystic Quest?\nApparently, it was made in an attempt to simplify Final Fantasy for the western market.\nThey were right, I suck at RPGs.",
    "Have you tried Heretic?\nWait, there is a Doom Engine game where you can look UP AND DOWN???",
    "Have you tried Hollow Knight?\nAnother independent hit revolutionising a genre!",
    "Have you tried Hylics 2?\nStop motion might just be the epitome of unique art styles.",
    "Have you tried Kirby's Dream Land 3?\nAll good things must come to an end, including Nintendo's SNES library.\nWent out with a bang though!",
    "Have you tried Kingdom Hearts?\nI'll wait for you to name a more epic crossover.",
    "Have you tried Kingdom Hearts II?\nIt's the third game in the epic crossover series.\n...wait...",
    "Have you tried Link's Awakening DX?\nHopefully, Link won't be obsessed with circles when he wakes up.",
    "Have you tried Landstalker?\nThe Witness player's greatest fear: A diagonal movement grid...\nWait, I guess we have the Monastery puzzles.",
    "Have you tried Lingo?\nIt's an open world puzzle game. It features puzzle panels with non-verbally explained mechanics.\nIf you like this game, you'll like Lingo too.",
    "(Middle Yellow)\nYOU AILED OVERNIGHT\nH--- --- ----- -----?",
    "Have you tried Lufia II?\nRoguelites are not just a 2010s phenomenon, turns out.",
    "Have you tried Mario & Luigi: Superstar Saga?\nSurely you're curious what all those \"Chuckle Beans\" you're collecting actually do.",
    "Have you tried Meritous?\nYou should know that obscure games are often groundbreaking!",
    "Have you tried Mega Man 2?\nI did once, but I still can't beat Air Man.",
    "Have you tried The Messenger?\nOld ideas made new again. It's how all art is made.",
    "Have you tried Minecraft?\nI have recently learned this is a question that needs to be asked.",
    "Have you tried Mega Man Battle Network 3?\nIt's a Mega Man RPG. How could you not want to try that?",
    "Have you tried Muse Dash?\nRhythm game with cute girls!\n(Maybe skip if you don't like the Jungle panels)",
    "Have you tried Noita?\nIf you like punishing yourself, you will like it.",
    "Have you tried Old School RuneScape?\n",
    "Have you tried Ocarina of Time?\nOne of the biggest randomizers, big inspiration for this one's features!",
    "Have you tried Old School Runescape?\nIt's the first AP game to be considered async-nonviable!",
    "Have you tried Overcooked 2?\nWhen you're done relaxing with puzzles, use your energy to yell at your friends.",
    "Have you tried Pokemon Emerald?\nI'm going to say it: 10/10, just the right amount of water.",
    "Have you tried Pokemon Red&Blue?\nA cute pet collecting game that fascinated an entire generation.",
    "Have you tried Raft?\nHaven't you always wanted to explore the ocean surrounding this island?",
    "Have you tried Rogue Legacy?\nAfter solving so many puzzles it's the perfect way to rest your \"thinking\" brain.",
    "Have you tried Risk of Rain 2?\nI haven't either. But I hear it's incredible!",
    "Have you tried Sonic Adventure 2?\nIf the silence on this island is getting to you, there aren't many games more energetic.",
    "Have you tried Starcraft 2?\nUse strategy and management to crush your enemies!",
    "Have you tried Shivers?\nWitness 2 should totally feature a haunted museum.",
    "Have you tried A Short Hike?\nKnowing the audience of this randomizer, there's about a 50% chance you're a furry. So this game is probably for you.",
    "Have you tried Super Metroid?\nA classic game, yet still one of the best in the genre.",
    "Have you tried Super Mario 64?\n3-dimensional games like this owe everything to that game.",
    "Have you tried Super Mario World?\nI don't think I need to tell you that it is beloved by many.",
    "Have you tried SMZ3?\nWhy play one incredible game when you can play 2 at once?",
    "Have you tried Secret of Evermore?\nI haven't either. But I hear it's great!",
    "Have you tried Slay the Spire?\nExperience the thrill of combat without needing fast fingers!",
    "Have you tried Stardew Valley?\nThe Farming game that gave a damn. It's so easy to lose hours and days to it...",
    "Have you tried Subnautica?\nIf you like this game's lonely atmosphere, I would suggest you try it.",
    "Have you tried Terraria?\nA prime example of a survival sandbox game that beats the \"Wide as an ocean, deep as a puddle\" allegations.",
    "Have you tried Timespinner?\nEveryone who plays it ends up loving it!",
    "Have you tried The Legend of Zelda?\nIn some sense, it was the starting point of \"adventure\" in video games.",
    "Have you tried TUNC?\nWhat? No, I'm pretty sure I spelled that right.",
    "Have you tried TUNIC?\nRemember what discovering your first Environmental Puzzle was like?\nTUNIC will make you feel like that at least 5 times over.",
    "Have you tried Undertale?\nI hope I'm not the 10th person to ask you that. But it's, like, really good.",
    "Have you tried VVVVVV?\nExperience the essence of gaming distilled into its purest form!",
    "Have you tried Wargroove?\nI'm glad that for every abandoned series, enough people are yearning for its return that one of them will know how to code.",
    "Have you tried The Witness?\nOh. I guess you already have. Thanks for playing!",
    "Have you tried Yacht Dice?\nIt's always fascinating how they turn simple clubhouse games into full AP implementations.",
    "Have you tried Yoshi's Island?\nSome might say it's the best 2D platformer ever made.\nBy which I mean me. I'm saying that. Go play it >:(((",
    "Have you tried Yu-Gi-Oh 2006?\nTry playing Pot of Greed; it will let you activate two additional junk hints.",
    "Have you tried Zillion?\nMe neither. But it looks fun. So, let's try something new together?",
    "Have you tried Zork: Grand Inquisitor?\nThis 1997 game uses Z-Vision technology to simulate 3D environments.\nCome on, I know you wanna find out what \"Z-Vision\" is.",

    "Quaternions break my brain",
    "Eclipse has nothing, but you should do it anyway.",
    "Beep",
    "Putting in custom subtitles shouldn't have been as hard as it was...",
    "BK mode is right around the corner.",
    "You can do it!",
    "I believe in you!",
    "The person playing is cute. <3",
    "dash dot, dash dash dash,\ndash, dot dot dot dot, dot dot,\ndash dot, dash dash dot",
    "When you think about it, there are actually a lot of bubbles in a stream.",
    "Never gonna give you up\nNever gonna let you down\nNever gonna run around and desert you",
    "Thanks to the Archipelago developers for making this possible.",
    "One day I was fascinated by the subject of generation of waves by wind.",
    "I don't like sandwiches. Why would you think I like sandwiches? Have you ever seen me with a sandwich?",
    "Where are you right now?\nI'm at soup!\nWhat do you mean you're at soup?",
    "Remember to ask in the Archipelago Discord what the Functioning Brain does.",
    "Don't use your puzzle skips, you might need them later.",
    "For an extra challenge, try playing blindfolded.",
    "Go to the top of the mountain and see if you can see your house.",
    "Yellow = Red + Green\nCyan = Green + Blue\nMagenta = Red + Blue",
    "Maybe that panel really is unsolvable.",
    "Did you make sure it was plugged in?",
    "Do not look into laser with remaining eye.",
    "Try pressing Space to jump.",
    "The Witness is a Doom clone.\nJust replace the demons with puzzles",
    "Test Hint please ignore",
    "Shapers can never be placed outside the panel boundaries, even if subtracted.",
    "The Keep laser panels use the same trick on both sides!",
    "Can't get past a door? Try going around. Can't go around? Try building a nether portal.",
    "We've been trying to reach you about your car's extended warranty.",
    "I hate this game. I hate this game. I hate this game.\n- Chess player Bobby Fischer",
    "Dear Mario,\nPlease come to the castle. I've baked a cake for you!",
    "Have you tried waking up?\nYeah, me neither.",
    "Why do they call it The Witness, when wit game the player view play of with the game.",
    "THE WIND FISH IN NAME ONLY, FOR IT IS NEITHER",
    "Like this game?\nTry The Wit.nes, Understand, INSIGHT, Taiji What the Witness?, and Tametsi.",
    "In a race, It's survival of the Witnesst.",
    "This hint has been removed. We apologize for your inconvenience.",
    "O-----------",
    "Circle is draw\nSquare is separate\nLine is win",
    "Circle is draw\nStar is pair\nLine is win",
    "Circle is draw\nCircle is copy\nLine is win",
    "Circle is draw\nDot is eat\nLine is win",
    "Circle is start\nWalk is draw\nLine is win",
    "Circle is start\nLine is win\nWitness is you",
    "Can't find any items?\nConsider a relaxing boat trip around the island!",
    "Don't forget to like, comment, and subscribe.",
    "Ah crap, gimme a second.\n[papers rustling]\nSorry, nothing.",
    "Trying to get a hint? Too bad.",
    "Here's a hint: Get good at the game.",
    "I'm still not entirely sure what we're witnessing here.",
    "Have you found a red page yet? No? Then have you found a blue page?",
    "And here we see the Witness player, seeking answers where there are none-\nDid someone turn on the loudspeaker?",

    "Be quiet. I can't hear the elevator.",
    "Witness me.\n- The famous last words of John Witness.",
    "It's okay, I always have to skip the Rotated Shaper puzzles too.",
    "Alan please add hint.",
    "Rumor has it there's an audio log with a hint nearby.",
    "In the future, war will break out between obelisk_sides and individual EP players.\nWhich side are you on?",
    "Droplets: Low, High, Mid.\nAmbience: Mid, Low, Mid, High.",
    "Name a better game involving lines. I'll wait.",
    "\"You have to draw a line in the sand.\"\n- Arin \"Egoraptor\" Hanson",
    "Have you tried?\nThe puzzles tend to get easier if you do.",
    "Sorry, I accidentally left my phone in the Jungle.\nAnd also all my fragile dishes.",
    "Winner of the \"Most Irrelevant PR in AP History\" award!",
    "I bet you wish this was a real hint :)",
    "\"This hint is an impostor.\"- Junk hint submitted by T1mshady.\n...wait, I'm not supposed to say that part?",
    "Wouldn't you like to know, weather buoy?",
    "Give me a few minutes, I should have better material by then.",
    "Just pet the doggy! You know you want to!!!",
    "ceci n'est pas une metroidvania",
    "HINT is MELT\nYOU is HOT",
    "Who's that behind you?",
    ":3",
    "^v ^^v> >>^>v\n^^v>v ^v>> v>^> v>v^",
    "Statement #0162601, regarding a strange island that--\nOh, wait, sorry. I'm not supposed to be here.",
    "Hollow Bastion has 6 progression items.\nOr maybe it doesn't.\nI wouldn't know.",
    "Set your hint count lower so I can tell you more jokes next time.",
    "A non-edge start point is similar to a cat.\nIt must be either inside or outside, it can't be both.",
    "What if we kissed on the Bunker Laser Platform?\nJk... unless?",
    "You don't have Boat? Invisible boat time!\nYou do have boat? Boat clipping time!",
    reinterpret_cast <const char*>(u8"Cet indice est en français. Nous nous excusons de tout inconvénients engendrés par cela."),
    "How many of you have personally witnessed a total solar eclipse?",
    "In the Treehouse area, you will find 69 progression items.\nNice.\n(Source: Just trust me)",
    "Lingo\nLingoing\nLingone",
    "The name of the captain was Albert Einstein.",
    "Panel impossible Sigma plz fix",
    "Welcome Back! (:",
    "R R R U L L U L U R U R D R D R U U",
    "Have you tried checking your tracker?",
    "Lines are drawn on grids\nAll symbols must be obeyed\nIt's snowing on Mt. Fuji",
    "If you're BK, you could try today's Wittle:\nhttps://www.fourisland.com/wittle/",
    "They say that plundering Outside Ganon's Castle is a foolish choice.",
    "You should try to BLJ. Maybe that'll get you through that door.",
    "Error: Witness Randomizer disconnected from Archipelago.\n(lmao gottem)",
    "You have found: One (1) Audio Log!\nSeries of 49! Collect them all!",
    "In the Town area, you will find 1 good boi.\nGo pet him.",
    "If you're ever stuck on a panel, feel free to ask Rever.\nSurely you'll understand his drawing!",
    "[This hint has been removed as part of the Witness Protection Program]",
    "Panel Diddle",
    "Witness AP when",
    "This game is my favorite walking simulator.",
    "Did you hear that? It said --\n\nCosmic background radiation is a riot!",
    "Well done solving those puzzles.\nPray return to the Waking Sands.",
    "Having trouble finding your checks?\nTry the PopTracker pack!\nIt's got auto-tracking and a detailed map.",
    "If you can't see the jungle for the trees, perhaps you should leaf.",
    "Every puzzle has exactly 5 different solutions.\nThat doesn't seem right? That's because it's not!",
    "Did you know you can turn off the audio logs by clicking on them again?",
    "oOwOo *notices your audiolog*",
    "This radon brought to you by ETAOIN SHRDLU",
    "Please ignore the previous hint, that was meant for someone else. Networking is so difficult sometimes...",
    "Oh, nice! I always forget to check this one.",
    "Progressive Dots can be found. Probably. Unless they can't.",
    "Don't know why the Mountain King, Mountain King, Mountain King,\nDon't know why the Mountain King has such a stressful halllll...",
    "Fun Fact: If you laid out every line you've drawn end-to-end, then that would be weird. Why did you do that? That's not how this game works.",
    "Sorry, this hint was removed by an Eraser.",
    "I hear the Elevator is a super handy shortcut if you forgot anything in the Tutorial area. :)",
    "That thing you just found can be found in the location you just found it.",
    "If you stand in just the right spot, that pile of rocks will look like a completely different pile of rocks.",
    "Ouagadougou can be found by visiting Burkina Faso.",
    "1 6 5 1 6 5 7 4 2 1\n... Oh, sorry, wrong game.",
    "Welcome to Thekla's modern remaster of the 1983 Infocom classic \"The Witness\"! Can YOU crack the murder mystery before sunrise?",
    "O----- --------\n(Your angle's off. Scooch over a bit.)",
    "What are you doing all the way over here?\nCloud Cycle is almost ready!",
    "This audio log felt pain when you clicked on it.\nBut it had no voice with which to scream.\nJust a single, pre-recorded message.",
    "Exactly one of these hints is a lie. Can you guess which one it is?",
    "Up there, you go around once every second\n- astronaut on a very fast space station",
    "Witness players be like \"sorry i can't get your moon pearl i need three more progressive squiggles and also my negative rotated eraser :(\"",
    "Rotated Shapers can be found somewhere (at some point).",
    "The more you bug Violet about double rando, the sooner it will come.\nTrust me.",
    "The Linghost will get you.\nRun.",
    "It's a secret to everybody.",
    "Just do the right check lol",
    "For the record, this is a completely separate game from \"The Wortness\".",

    "Obligatory \"Todo: Add Hint\" hint",
    "The red eight goes on the black nine.",
    "That's right, it goes in the square hole!",
    "Where do the triangles go?\nThat's right!\nIn the square hole!",
    "Has anyone seen the linking book off this island?",
    "When the island is insightful",
    "youtu.be/YgzrslSRdjI",
    "Fun fact: Your player model's name is Carl!",
    "Why is there no cat statue? Kitty deserves pets too!",
    "Is The Witness is a Yume Nikki fangame?",
    "Up Left Up Right Down Right\n*Fairy noises*",
    "Have you referenced The Witness in an essay?\nIs the citation format Last Name, First Initial?\nI'm so sorry.",
    "Sometimes, I dream about cheese... Wait, this is a \"junk\" hint? Rude.",
    "No, I have NOT tried that. Stop telling me to try new things!",
    "Wait. Hang on. I think I just figured out the answer to that one puzzle.",
    "the lookerrrrrr",
    "Have you tried submitting your own junk hints? Check the pinned messages in the Discord to find out how!",
    "17 audio logs from now, there is going to be an untrue hint.\nThis carries over across multiworlds.\nYou've been warned.",
    "Help, I'm trapped in a hint factory!",
    "Legends say that there is an actual map on the island that keeps track of your progress.",
    "Pro tip: Changing your FOV while solving puzzles can clip through obstruction.\nTry using it for Burnt Treehouse EP and Black Line Tower EP!",

    "Did you know?\nIt's possible to get free hints by clicking on audio logs.",
    "The average puzzler typically solves things using lines and shapes, but what happens if they are forced to use their words? In this study...",
    "At least 1 progression item can be found in your world. Probably.",
    "Exempt have you checked Bunker EPs?",

    "Hints suggested by:\nIHNN, Beaker, MrPokemon11, Ember, TheM8, NewSoupVi, Jasper Bird, T1mshady, KF, Yoshi348, Berserker, BowlinJim, oddGarrett, Pink Switch, Rever, Ishigh, snolid, CodeGorilla, Quas NaArt, Scipio, Projectyl, ManyPinkHats, m4elstrom, Kinrah, TG, transmothgirl, Aplle, Orsan.",
  };

  return jokeHints;
}
