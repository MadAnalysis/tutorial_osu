# Example sample

For this tutorial we used MadGraph `v3.3.1` and generated a boosted $t\bar{t}$ event. The boost cuts are defined in 
`cuts.f` (simply search for `TUTO_OSU`). This boost will allow us to populate high mass regions sufficiently 
without the need of generating too many events. `proc_card_mg5.dat` shows the event generation process. To get
the same event efficiency make sure to copy `cuts.f` file to `Subprocess` folder in MadGraph sample folder (optional). 
`run_card.dat` defines other base cuts. 

Please make sure that you have generated at least 10K showered and hadronised events before attending to the tutorial.
The results have been prepared with 50K events, but to test your code during the tutorial session we suggest
generating a small sample to run quickly.