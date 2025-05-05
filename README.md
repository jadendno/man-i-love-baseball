# Man, I Love Baseball

A graphical baseball vacaction planner. Allows you to plan a trip to any Major League Baseball (MLB)-associated stadium.

Project for CS 008: Data Structures and Algorithms with Juan Leon.



View Alphabetically Sorted Stadium List
Description:
As a fan, I want to view all stadiums in alphabetical order by name so I can easily browse them.

Tasks:
Load stadium data from the input.
Sort by stadium name.
Display a sorted list.
Test:
Load sample data and verify the order starts with "Angels Stadium..." and ends with "Yankee Stadium".


Sort Stadiums by Team Name
Description:
As a fan, I want to see stadiums sorted by team name to find my favorite team's stadium quickly.
Tasks:
Add sorting logic based on team name.
Display the results.
Test:
Verify "Boston Red Sox" stadium appears before the "Detroit Tigers".


Plan Shortest Trip to All Stadiums
Description:
As a fan, I want to plan the shortest trip to all stadiums starting from California so I can minimize travel distance.
Tasks:
Implement distance matrix.
Apply TSP (Traveling Salesman Problem) approximation.
Generate trip route and total distance.
Test:
Verify that the output starts from a California stadium and returns a reasonable short path.

View Stadiums by League
Description:
As a fan, I want to filter stadiums by American or National League so I can focus my trip planning.
Tasks:
Tag each stadium with the league.
Add filter logic.
Display filtered lists.
Test:
Verify that only AL teams show when "American League" is selected.

Track Souvenir Purchases
Description:
As a fan, I want to track souvenirs I purchase during trips so I can manage my expenses.
Tasks:
Create a souvenir list and pricing.
Store user purchases.
Sum total cost.
Test:
Add a cap and bat to the cart, verify total = $25.99 + $35.35.
Admin Can Modify Stadium Info
Description:
As an admin, I want to update stadium data (e.g., team moves) so the app stays current.
Tasks:
Add admin access control.
Enable edit functionality.
Save updates persistently.
Test:
Update seating capacity for "Fenway Park" and verify it's stored.

Admin Can Add a New Team/Stadium
Description:
As an admin, I want to add new teams and stadiums in case the league expands.
Tasks:
Input form for new team/stadium.
Validate inputs.
Store persistently.
Test:
Add "Las Vegas Vipers" and verify it appears in all lists.

Modify Souvenir List (Admin Only)
Description:
As an admin, I want to modify souvenir items and prices so I can reflect changes in the store.
Tasks:
Add item management UI.
Store in editable format.
Restrict to admin.
Test:
Change "Baseball bat" price to $40.00 and verify the update.

Show Distance Between Any Two Stadiums
Description:
As a fan, I want to see the distance between any two stadiums so I can plan individual legs.
Tasks:
Allow stadium pair selection.
Calculate the distance from the matrix.
Test:
Input "Fenway Park" and "Yankee Stadium", verify correct mileage.

Display Trip Map with Highlighted Path
Description:
As a fan, I want to see a visual map with my route highlighted so I can visualize the journey.
Tasks:
Integrate a basic map or a static image.
Draw route overlay.
Sync with trip planner.
Test:
Plan a trip from CA, and check if all stadiums are connected in order visually.

View Stadiums by Date Opened
Description:
As a fan, I want to see stadiums sorted by the date they opened so I can explore historical parks.
Tasks:
Parse and format stadium opening dates.
Implement sorting logic.
Display results.
Test:
Verify that "Fenway Park (1912)" appears before "Target Field (2010)".

View Stadiums with Grass Surface
Description:
As a fan, I want to view only stadiums with grass surfaces so I can plan a natural turf tour.
Tasks:
Tag stadiums by surface type.
Add filter logic.
Display filtered list.
Test:
Filter by "grass" and verify that artificial turf stadiums are excluded.

Search for Stadium by Name or Team
Description:
As a user, I want to search for a stadium by name or team so I can find details quickly.
Tasks:
Create an input field for search.
Implement partial match search.
Show result or “not found” message.


Test:
Search "Dodgers", verify Dodger Stadium is returned.

View Stadium Details
Description:
As a user, I want to click a stadium and view full details (address, phone, capacity) so I can learn more.
Tasks:
Design a detailed view layout.
Populate fields from data.
Connect with the stadium list view.
Test:
Click "Yankee Stadium" and verify that all stored info appears.

Admin Can Delete the Stadium or the Team
Description:
As an admin, I want to delete a team and its stadium if it's no longer active.
Tasks:
Add delete option (admin-only).
Confirm deletion.
Update all views accordingly.
Test:
Delete "Oakland Athletics", confirm it’s removed from all lists.

