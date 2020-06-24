{
	TFile f("neutron_beam_25meV_1.5mm_LUT_polishedair.root", "READ");
	if (f.IsOpen()) {
		printf("File open successfully\n");
	}

	gDirectory->ls("-d");
	auto t = (TTree*) f.Get("Hits");
	// printf("%d\n",t->GetNBranches());

	f.Close();

}